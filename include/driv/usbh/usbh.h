/*
 * usbh.h
 *
 *  Created on: 9 nov. 2017
 *      Author: SERAGUD
 */

#ifndef INCLUDE_DRIV_USBH_USBH_H_
#define INCLUDE_DRIV_USBH_USBH_H_

#include "types.h"

typedef enum {
	Usbh_tokenPid_out = 0x1,
	Usbh_tokenPid_in = 0x9,
	Usbh_tokenPid_sof = 0x5,
	Usbh_tokenPid_setup = 0xD,
	Usbh_tokenPid_data0 = 0x3,
	Usbh_tokenPid_data1 = 0xB,
	Usbh_tokenPid_data2 = 0x7,
	Usbh_tokenPid_mdata = 0xF,
	Usbh_tokenPid_ack = 0x2,
	Usbh_tokenPid_nack = 0xA,
	Usbh_tokenPid_stall = 0xE,
	Usbh_tokenPid_nyet = 0x6,
	Usbh_tokenPid_pre = 0xC,
	Usbh_tokenPid_err = 0xC,
	Usbh_tokenPid_split = 0x8,
	Usbh_tokenPid_ping = 0x4,
} usbh_tokenPid_t;

/// Values for descriptor type
typedef enum {
	Usbh_descType_device = 1,
	Usbh_descType_configuration = 2,
	Usbh_descType_string = 3,
	Usbh_descType_interface = 0x04U,
	Usbh_descType_endpoint = 0x05U,
	Usbh_descType_deviceQualifier = 0x06U,
	Usbh_descType_otherSpeedConfiguration = 0x07,
	Usbh_descType_interfacePower = 0x08,
	Usbh_descType_otg = 0x09,
	Usbh_descType_interfaceAssociation = 0x0B,
	Usbh_descType_bos = 0x0F,
	Usbh_descType_deviceCapability = 0x10,
} usbh_descType_t;

/// Setup packet's request type
typedef enum {
	Usbh_reqType_dir_Shift = 7,
	Usbh_reqType_dir_Mask = 1 << Usbh_reqType_dir_Shift,
	Usbh_reqType_dir_out = 1 << Usbh_reqType_dir_Shift,
	Usbh_reqType_dir_in = 1 << Usbh_reqType_dir_Shift,

	Usbh_reqType_type_Shift = 5,
	Usbh_reqType_type_Mask = 3 << Usbh_reqType_type_Shift,
	Usbh_reqType_type_standard = 0 << Usbh_reqType_type_Shift,
	Usbh_reqType_type_class = 1 << Usbh_reqType_type_Shift,
	Usbh_reqType_type_vendor = 2 << Usbh_reqType_type_Shift,

	Usbh_reqType_recipient_Mask = 0x1F,
	Usbh_reqType_recipient_device = 0,
	Usbh_reqType_recipient_interface = 1,
	Usbh_reqType_recipient_endpoint = 2,
	Usbh_reqType_recipient_other = 3,
} usbh_setupPacketReqType_t;

typedef enum {
	Usbh_standardReq_getStatus = 0x00,
	Usbh_standardReq_clearFeature = 0x01,
	Usbh_standardReq_setFeature = 0x03,
	Usbh_standardReq_setAddress = 0x05,
	Usbh_standardReq_getDescriptor = 0x06,
	Usbh_standardReq_setDescriptor = 0x07,
	Usbh_standardReq_getConfiguration = 0x08,
	Usbh_standardReq_setConfiguration = 0x09,
	Usbh_standardReq_getInterface = 0x0A,
	Usbh_standardReq_setInterface = 0x0B,
	Usbh_standardReq_synchFrame = 0x0C,
} usbh_standardReq_t;

/// Model to hold setup packet
typedef struct {
	/// Setup request type
	///
	/// D7 Data Phase Transfer Direction
	/// 0 = Host to Device
	/// 1 = Device to Host
	/// D6..5 Type
	/// 0 = Standard
	/// 1 = Class
	/// 2 = Vendor
	/// 3 = Reserved
	/// D4..0 Recipient
	/// 0 = Device
	/// 1 = Interface
	/// 2 = Endpoint
	/// 3 = Other
	/// 4..31 = Reserve
	uint8_t bmRequestType;
	/// TODO what is it?
	uint8_t bRequest;
	/// TODO what is it?
	uint16_t wValue;
	/// TODO what is it?
	uint16_t wIndex;
	/// Number of bytes to be transferred
	uint16_t wLength;
} usbh_setupPacket_t;

/// Model for USB's device descriptor
typedef struct _usb_descriptor_device {
	/// Size of this descriptor in bytes
	uint8_t bLength;
	/// DEVICE Descriptor Type
	uint8_t bDescriptorType;
	/// UUSB Specification Release Number in Binary-Coded Decimal, e.g. 0x0200U
	uint8_t bcdUSB[2];
	/// Class code
	uint8_t bDeviceClass;
	/// Sub-Class code
	uint8_t bDeviceSubClass;
	/// Protocol code
	uint8_t bDeviceProtocol;
	/// Maximum packet size for endpoint zero
	uint8_t bMaxPacketSize0;
	/// Vendor ID (assigned by the USB-IF)
	uint8_t idVendor[2];
	/// Product ID (assigned by the manufacturer)
	uint8_t idProduct[2];
	/// Device release number in binary-coded decimal
	uint8_t bcdDevice[2];
	/// Index of string descriptor describing manufacturer
	uint8_t iManufacturer;
	/// Index of string descriptor describing product
	uint8_t iProduct;
	/// Index of string descriptor describing the device serial number
	uint8_t iSerialNumber;
	/// Number of possible configurations
	uint8_t bNumConfigurations;
} usbh_device_descriptor_t;

/// Model for USB's configuration descriptor
typedef struct {
	uint8_t bLength; /* Descriptor size in bytes = 9U */
	uint8_t bDescriptorType; /* CONFIGURATION type = 2U or 7U */
	uint8_t wTotalLength[2]; /* Length of concatenated descriptors */
	uint8_t bNumInterfaces; /* Number of interfaces, this configuration. */
	uint8_t bConfigurationValue; /* Value to set this configuration. */
	uint8_t iConfiguration; /* Index to configuration string */
	uint8_t bmAttributes; /* Configuration characteristics */
	uint8_t bMaxPower; /* Maximum power from bus, 2 mA units */
} usbh_configuration_descriptor_t;

/// Model for USB's interface descriptor
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} usbh_interface_descriptor_t;

/// Model for USB's endpoint descriptor
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint8_t wMaxPacketSize[2];
	uint8_t bInterval;
} usbh_endpoint_descriptor_t;

typedef enum {
	Usbh_endpointType_control = 0,
	Usbh_endpointType_isochronous = 1,
	Usbh_endpointType_bulk = 2,
	Usbh_endpointType_interrupt = 3,
} usbh_endpoint_type_t;

typedef enum {
	Usbh_class_msd = 8,
} usbh_class_t;

/// Initializes USB host interface
extern bool_t usbh_init(void);

typedef enum {
	Usbh_eventResp_used = 0,
	Usbh_eventResp_noError = 1,
} usbh_eventResp_t;

/// Must be implemented by the application to notify USBH Class device drivers
/// that a new device has been attached.
extern usbh_eventResp_t usbh_event_attached(void);

/// Must be implemented by the application to notify USBH Class device drivers
/// that a new device has been enumerated.
extern usbh_eventResp_t usbh_event_enumerationDone(void);

extern usbh_eventResp_t usbh_event_detached(void);

#endif /* INCLUDE_DRIV_USBH_USBH_H_ */
