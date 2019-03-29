#include "usb_serial.h"
#include <stdio.h>
#include <string.h>
#include <varargs.h>

//#define USE_USBDK

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define snprintf _snprintf
#endif

void usb_serial_init_libusb(libusb_context** context, int debug)
{
	libusb_context** ctx = NULL;
	int error = libusb_init(ctx);
	if (error < 0)
	{
		fprintf_s(stderr, "Error initializing lib_usb: %#08X\n", error);
		return;
	}
	else if(debug != 0) //Use debug options
	{
		int op_err = libusb_set_option(ctx, USB_SERIAL_LIBUSB_OPTION_LOG_LEVEL, USB_SERIAL_LIBUSB_LOG_LEVEL_DEBUG);
		if (op_err == LIBUSB_ERROR_INVALID_PARAM || op_err == LIBUSB_ERROR_NOT_SUPPORTED)
		{
			fprintf_s(stderr, "libusb: Invalid option or not supported. Error: %#08X\n", op_err);
		}
	}
	else //Use default options
	{
		int op_err = libusb_set_option(ctx, USB_SERIAL_LIBUSB_OPTION_LOG_LEVEL, USB_SERIAL_LIBUSB_LOG_LEVEL_ERROR);
		if (op_err == LIBUSB_ERROR_INVALID_PARAM || op_err == LIBUSB_ERROR_NOT_SUPPORTED)
		{
			fprintf_s(stderr, "libusb: Invalid option or not supported. Error: %#08X\n", op_err);
		}
	}

#if defined (_WIN32) && defined (USE_USBDK)//try to use usbdk under windows
	int op_err = libusb_set_option(ctx, USB_SERIAL_LIBUSB_OPTION_USE_USBDK);
	if (op_err == LIBUSB_ERROR_INVALID_PARAM || op_err == LIBUSB_ERROR_NOT_SUPPORTED)
	{
		fprintf_s(stderr, "libusb: Invalid option or not supported. Error: %#08X\n", op_err);
	}
#endif
}

void usb_serial_deinit_libusb(libusb_context** context)
{
	libusb_exit(context);
}

void print_device(libusb_device* dev, int level)
{
	struct libusb_device_descriptor desc;
	libusb_device_handle* handle = NULL;
	char description[256];
	char string[256];
	int dd_ret;
	uint8_t i;

	dd_ret = libusb_get_device_descriptor(dev, &desc);
	if (dd_ret < 0)
	{
		fprintf_s(stderr, "libusb: Failed to get device descriptor. Error: %#08X\n", dd_ret);
	}
	dd_ret = libusb_open(dev, &handle);
	if (LIBUSB_SUCCESS == dd_ret) 
	{
		if (desc.iManufacturer) 
		{
			dd_ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
			if (dd_ret > 0)
				snprintf(description, sizeof(description), "%s - ", string);
			else
				snprintf(description, sizeof(description), "%04X - ", desc.idVendor);
		}
		else
			snprintf(description, sizeof(description), "%04X - ", desc.idVendor);

		if (desc.iProduct) 
		{
			dd_ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, sizeof(string));
			if (dd_ret > 0)
				snprintf(description + strlen(description), sizeof(description) - strlen(description), "%s", string);
			else
				snprintf(description + strlen(description), sizeof(description) - strlen(description), "%04X", desc.idProduct);
		}
		else
			snprintf(description + strlen(description), sizeof(description) - strlen(description), "%04X", desc.idProduct);
	}
	else 
	{
		snprintf(description, sizeof(description), "%04X - %04X", desc.idVendor, desc.idProduct);
	}

	printf("%.*sDev (bus %d, device %d): %s\n", level * 2, "                    ", 
		libusb_get_bus_number(dev), libusb_get_device_address(dev), description);
}

//device discovery
void usb_serial_list_devices(libusb_context ** context)
{
	libusb_device ** list;
	ssize_t cnt = libusb_get_device_list(context, &list);
	int error = 0;

	if (cnt < 0)
	{
		fprintf_s(stderr, "libusb: Invalid device list discovery. Error: %#08X\n", error);
		return;
	}
	else
	{
		putchar('\n');
		printf("USB Serial Devices List\n");
		for (int i = 0; i < cnt; i++)
		{
			libusb_device * device = list[i];
			print_device(device, 0);
		}
		putchar('\n');
	}

	libusb_free_device_list(list, 1);
}
