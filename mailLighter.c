#include <libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ID_VENDOR 0x1294
#define ID_PRODUCT 0x1320

int main(int argc, char * argv[]) {

    libusb_device_handle * handle;
    int actual;
    unsigned char data[8];

    if(argv[1] == NULL) {
        printf("Usage: mailLighter color(1-7, 0-off)\n");
        return 1;
    }

    libusb_init(NULL);

    handle = libusb_open_device_with_vid_pid(NULL, ID_VENDOR, ID_PRODUCT);

    if (handle == NULL) {
        printf("MailNotifierDevice not found\n");
        libusb_exit(NULL);
        return 0;
    }

    if (libusb_kernel_driver_active(handle,0)) {
    	libusb_detach_kernel_driver(handle, 0);
    }

    if (libusb_claim_interface(handle, 0) < 0) {
        printf("Interface error\n");
        libusb_exit(NULL);
        return 0;
    }

    *data = atoi(argv[1]);

    libusb_interrupt_transfer(handle, 0x02, data, sizeof(data), &actual, 100);

    libusb_attach_kernel_driver(handle, 0);
    libusb_close(handle);
    libusb_exit(NULL);
    return 0;

}