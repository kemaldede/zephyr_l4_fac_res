
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <services/custom_service.h>
#include <utils/rssi.h>

#define BT_UUID_CUSTOM_SERVICE BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x10, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA0)
// input to the peripheral device
#define BT_UUID_INPUT BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x11, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA1)
// output from the peripheral device
#define BT_UUID_OUTPUT BT_UUID_DECLARE_128(0x3E, 0x09, 0x99, 0x11, 0x29, 0x3F, 0x11, 0xE4, 0x93, 0xBD, 0xAF, 0xD0, 0xFE, 0x6D, 0x1D, 0xA2)

// BT_GATT_CHARACTERISTIC(_uuid, _props, _perm, _read, _write, _value)

static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERVICE),
		BT_GATT_CHARACTERISTIC(BT_UUID_INPUT, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE_AUTHEN, NULL, write_input, NULL),
		BT_GATT_CHARACTERISTIC(BT_UUID_OUTPUT, BT_GATT_CHRC_READ, BT_GATT_PERM_READ_ENCRYPT, read_output, NULL, NULL),
};

static struct bt_gatt_service custom_service = BT_GATT_SERVICE(attrs);

uint8_t a_number = 64;

void custom_service_init()
{
	bt_gatt_service_register(&custom_service);
}

ssize_t write_input(struct bt_conn *conn,
										const struct bt_gatt_attr *attr,
										const void *buf, uint16_t len, uint16_t offset,
										uint8_t flags)
{

    // TODO authorisation check - only allow writes when the Central and (this) Peripheral device are nice and close to each other
	// printk("Checking RSSI\n");
	// int8_t rssi = get_rssi();
	// printk("RSSI: %01hhd \n",rssi);
	// if (rssi < -50) 
	// {
	// 	printk("devices not close enough – requested operation not authorized\n");
	// 	return BT_GATT_ERR(BT_ATT_ERR_AUTHORIZATION);
	// }

	const uint8_t *new_number = buf;
	if (!len)
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (*new_number >= 0 && *new_number <= 10)
	{
		a_number = *new_number;
	}
	else
	{
		return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
	}

	printk("write_input [%d]\n", a_number);
	return len;
}

ssize_t read_output(struct bt_conn *conn,
										const struct bt_gatt_attr *attr, void *buf,
										uint16_t len, uint16_t offset)
{
	printk("read_output\n");

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &a_number, sizeof(a_number));
}
