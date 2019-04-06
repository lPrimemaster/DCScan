#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_
#include <libusb.h>
#include <stdio.h>

//redefine lib_usb flags ... hidding
enum usb_serial_options
{
	USB_SERIAL_LIBUSB_OPTION_LOG_LEVEL = 0,
	USB_SERIAL_LIBUSB_OPTION_USE_USBDK = 1,

	USB_SERIAL_LIBUSB_LOG_LEVEL_NONE = 0,
	USB_SERIAL_LIBUSB_LOG_LEVEL_ERROR = 1,
	USB_SERIAL_LIBUSB_LOG_LEVEL_WARNING = 2,
	USB_SERIAL_LIBUSB_LOG_LEVEL_INFO = 3,
	USB_SERIAL_LIBUSB_LOG_LEVEL_DEBUG = 4
};

typedef struct usb_serial_device_list
{
	libusb_device** list;
	ssize_t count;
}usb_serial_device_list;

#ifdef __cplusplus
extern "C" {
#endif
void usb_serial_init_libusb(libusb_context** context, int debug);
void usb_serial_deinit_libusb(libusb_context** context);
void usb_serial_list_devices(libusb_context** context);

void usb_serial_get_device_list(libusb_context ** context, usb_serial_device_list * list);
void usb_serial_free_device_list(usb_serial_device_list * list);
void usb_serial_print_device_list(usb_serial_device_list * list);

ssize_t usb_serial_get_device_index_from_id(usb_serial_device_list * list, uint16_t id);
ssize_t usb_serial_get_device_index_from_name(usb_serial_device_list * list, const char* name);
#ifdef __cplusplus
}
#endif

#endif /* USB_SERIAL_H_ */