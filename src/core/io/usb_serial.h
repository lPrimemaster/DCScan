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

#ifdef __cplusplus
extern "C" {
#endif
void usb_serial_init_libusb(libusb_context** context, int debug);
void usb_serial_deinit_libusb(libusb_context** context);
void usb_serial_list_devices(libusb_context** context);
#ifdef __cplusplus
}
#endif

#endif /* USB_SERIAL_H_ */