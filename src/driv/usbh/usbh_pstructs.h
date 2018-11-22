/// Declares private structures

#ifndef SRC_DRIV_USBH_USBH_PSTRUCTS_H_
#define SRC_DRIV_USBH_USBH_PSTRUCTS_H_

#include "driv/usbh/usbh.h"

enum {
	/// The max NAK count for one transaction.
	///
	/// When NAK count reach this value, the transaction fails.
	Usbh_config_maxNak = 3000U,

	Usbh_config_maxNakRetries = 1,

	/// The max retries for enumeration.
	Usbh_config_enumMaxRetries = 3,

	Usbh_config_timeoutNoData = 500,

	Usbh_config_timeoutToHost = 5000,

	Usbh_config_timeoutToDevice = 5000,

	Usbh_config_timeoutOther = 10000,

	Usbh_config_timeoutDefault = 500,

	/// The max retries for enumeration setup stall.
	Usbh_config_maxStallRetries = 1,

	/// Max size of transfer buffer
	Usbh_transferBuf_size = 512,

	/// Number of total pipes in USBH module
	Usbh_numPipesInModule = 5,

	/// Number of total pipes in USBH module
	Usbh_numTransfersInModule = 5,

	Usbh_maxTransferRetries = 3,
} priv_consts_t;

typedef enum {
	Usbh_pipeDir_out = 0, Usbh_pipeDir_in = 1,
} usbh_pipe_dir_t;

/// USB error code
typedef enum _usb_status
{
    Usbh_transferStatus_Success = 0x00U, /*!< Success */
    Usbh_transferStatus_Error,           /*!< Failed */

    Usbh_transferStatus_Busy,                       /*!< Busy */
    Usbh_transferStatus_InvalidHandle,              /*!< Invalid handle */
    Usbh_transferStatus_InvalidParameter,           /*!< Invalid parameter */
    Usbh_transferStatus_InvalidRequest,             /*!< Invalid request */
    Usbh_transferStatus_ControllerNotFound,         /*!< Controller cannot be found */
    Usbh_transferStatus_InvalidControllerInterface, /*!< Invalid controller interface */

    Usbh_transferStatus_NotSupported,   /*!< Configuration is not supported */
    Usbh_transferStatus_Retry,          /*!< Enumeration get configuration retry */
    Usbh_transferStatus_TransferStall,  /*!< Transfer stalled */
    Usbh_transferStatus_TransferFailed, /*!< Transfer failed */
    Usbh_transferStatus_AllocFail,      /*!< Allocation failed */
    Usbh_transferStatus_LackSwapBuffer, /*!< Insufficient swap buffer for KHCI */
    Usbh_transferStatus_TransferCancel, /*!< The transfer cancelled */
    Usbh_transferStatus_BandwidthFail,  /*!< Allocate bandwidth failed */
    Usbh_transferStatus_MSDStatusFail,  /*!< For MSD, the CSW status means fail */
    Usbh_transferStatus_EHCIAttached,
    Usbh_transferStatus_EHCIDetached,
} usbh_transferStatus_t;

/// States of device instances enumeration
typedef enum {
	/// Device is invalid
	Usbh_enumState_notInit = 0,
	/// Device has been processed by host driver
	Usbh_enumState_initial,
	/// Enumeration process: get 8 bytes' device descriptor
	Usbh_enumState_getDes8,
	/// Enumeration process: set device address
	Usbh_enumState_setAddress,
	/// Enumeration process: get device descriptor
	Usbh_enumState_getDes,
	/// Enumeration process: get 9 bytes' configuration descriptor
	Usbh_enumState_getCfg9,
	/// Enumeration process: get configuration descriptor
	Usbh_enumState_getCfg,
	/// Enumeration process: set configuration
	Usbh_enumState_setCfg,
	/// Enumeration is done
	Usbh_enumState_enumDone,
	/// This device has been used by application
	Usbh_enumState_appUsed,
} usbh_enumState_t;

/// State of transmitting a Setup transfer
typedef enum {
	Usbh_setupStatus_idle = 0,
	Usbh_setupStatus_0,
	Usbh_setupStatus_1,
	Usbh_setupStatus_2,
	Usbh_setupStatus_3,
} transfer_status_t;

typedef enum {
	Usbh_packetTranStatus_crcError = -4,
	/// Packet transmission got an NAK
	Usbh_packetTranStatus_nak = -256,
	/// Packet transmission timedout
	Usbh_packetTranStatus_timeout = -4096,
} usbh_packetTranStatus_t;

typedef enum
{
    Usbh_transactionType_ctrl = 0,
	Usbh_transactionType_in,
	Usbh_transactionType_out,
	Usbh_transactionType_outHndsk,
	Usbh_transactionType_inHndsk,
	Usbh_transactionType_unknown,
} usbh_transactionType_t;

/// Model to hold USB transfer state/information
///
/// If pipeNum == 0xFFFF, the transfer record is free.
typedef struct {
	/// The pipe this transfer belongs to.
	///
	/// If pipeNum == 0xFFFF, the transfer record is free.
	uint16_t pipeNum;

	/// Transfer data buffer
	uint8_t transferBuffer[Usbh_transferBuf_size];
	/// Transfer data length
	uint32_t transferLength;
	/// Length transferred so far
	uint32_t transferSofar;

	/// Transfer callback function
	// TODO host_inner_transfer_callback_t callbackFn;
	/// Set up packet buffer. TODO Can this be stored in transferBuffer
	usbh_setupPacket_t setupPacket;
	/// Transfer direction. In or Out.
	usbh_pipe_dir_t dir;
	/// Set up the transfer status
	transfer_status_t setupStatus;

	union {
		/// xTD head for this transfer
		uint32_t unitHead;
		/// KHCI transfer result
		int32_t transferResult;
	} union1;

	union {
		/// xTD tail for this transfer
		uint32_t unitTail;
		/// KHCI transfer frame number
		uint32_t frame;
	} union2;

	/// KHCI transfer NAK timeout
	uint16_t nakTimeout;
	/// KHCI transfer retry
	uint16_t retry;
} usbh_transfer_t;

/// USB host endpoint information structure
typedef struct {
	/// Endpoint descriptor pointer
	usbh_endpoint_descriptor_t epDesc;
	/// Endpoint extended descriptor pointer
	// TODO uint8_t *epExtension;
	/// Extended descriptor length
	uint16_t epExtensionLength;
} usbh_ep_t;

/// USB host interface information structure
typedef struct {
	/// Endpoint array
	usbh_ep_t epList[USB_HOST_CONFIG_INTERFACE_MAX_EP];
	/// Interface descriptor pointer
	usbh_interface_descriptor_t interfaceDesc;
	/// Interface extended descriptor pointer
	// TODO uint8_t *interfaceExtension;
	/// Extended descriptor length
	uint16_t interfaceExtensionLength;
	/// The interface index
	uint8_t interfaceIndex;
	/// The interface alternate setting value
	uint8_t alternateSettingNumber;
	/// Interface's endpoint number
	uint8_t epCount;
} usbh_interface_t;

/// USB host configuration information structure
typedef struct {
	/// The configuration's interface number
	uint8_t interfaceCount;
	/// Interface array
	usbh_interface_t interfaceList[USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE];

	/// Configuration descriptor pointer
	usbh_configuration_descriptor_t configurationDesc;
	/// Configuration extended descriptor pointer
	// TODO uint8_t *configurationExtension;
	/// Extended descriptor length
	uint16_t configurationExtensionLength;
} usbh_configuration_t;

/// USB host pipe common structure
typedef struct {
	// TODO Next pipe. Convert this an array in parent structure
	// TODO struct _usb_host_pipe *next;    /*!< Link the idle pipes*/

	/// For KHCI transfer. TODO: What is this?
	uint16_t currentCount;
	/// Maximum NAK count
	uint16_t nakCount;
	/// Maximum packet size
	uint16_t maxPacketSize;
	/// FS/LS: frame unit; HS: micro-frame unit. TODO: What is this?
	uint16_t interval;
	/// 0 - closed, 1 - open
	bool_t open:1;
	/// Determines which Data Token type next packet shall be.
	uint8_t nextdata01:1;
	/// Endpoint address
	uint8_t endpointAddr;
	/// Pipe direction
	usbh_pipe_dir_t dir;
	/// Pipe type, for example USB_ENDPOINT_BULK
	usbh_endpoint_type_t pipeType;
	/// Transaction number per micro-frame. TODO: What is this?
	uint8_t numberPerUframe;
} usbh_pipe_t;

typedef struct {
	/// Parsed configuration information for the device
	usbh_configuration_t configuration;
	/// Standard device descriptor
	// TODO usb_descriptor_device_t deviceDescriptor;

	/// Device's control pipe
	uint16_t controlPipeNum;

	// TODO uint8_t *configurationDesc;               /*!< Configuration descriptor pointer */
	/// Configuration descriptor length
	uint16_t configurationLen;
	/// Configuration index of the selected configuration
	uint16_t configurationValue;
	/// Interfaces' status, please reference to #usb_host_interface_state_t
	// TODO uint8_t interfaceStatus[USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE];

	bool_t isAttached;

	/// Buffer for enumeration
	uint8_t enumBuffer[9];
	/// Device state for enumeration.
	usbh_enumState_t state;
	/// Re-enumeration when error in control transfer
	uint8_t enumRetries;
	/// Re-transfer when stall
	uint8_t stallRetries;
	/// Device speed
	uint8_t speed;

	// Address

	/// Temporary address for the device. When set address request succeeds, setAddress is a value, 1 - 127
	uint8_t allocatedAddress;
	/// The address has been set to the device successfully, 1 - 127
	uint8_t setAddress;

	// Hub related

    uint8_t hubNumber;   /*!< Device's first connected hub address (root hub = 0) */
    uint8_t portNumber;  /*!< Device's first connected hub's port no (1 - 8) */
    uint8_t hsHubNumber; /*!< Device's first connected high-speed hub's address (1 - 8) */
    uint8_t hsHubPort;   /*!< Device's first connected high-speed hub's port no (1 - 8) */
    // Device's level (root device = 0)
    uint8_t level;
} usbh_device_t;

typedef enum {
	Usbh_transferState_getMsg = 0,
	Usbh_transferState_getIsoMsg,
	Usbh_transferState_startTransmit,
	Usbh_transferState_transmiting,
	Usbh_transferState_transmitDone,
	Usbh_transferState_none,
} usbh_transferState_t;

typedef struct {
	/// Holds state and information about the device.
	usbh_device_t dev;

	usbh_transferState_t transferState;

	/// Id of the current transfer
	uint16_t curTransfer;

	/// Determines if odd/even BD is used in TX buffer descriptor
	/// TODO Does it tell which of TX bd is free?
	uint8_t txBD :1;

	/// Determines if odd/even BD is used in RX buffer descriptor
	/// TODO Does it tell which of RX bd is free?
	uint8_t rxBD :1;

	bool_t deviceAttached:1;

	usbh_transfer_t transfers[Usbh_numTransfersInModule];

	usbh_pipe_t pipes[Usbh_numPipesInModule];

	uint32_t longFrameNumber;

	uint16_t oldFrameNumber;
} usbh_mod_t;

static usbh_mod_t me;

__attribute__((aligned(512)))
  static bdt_t bdt[16 * 2 * 2];

static inline uint16_t priv_compute_bdtIndex(bool_t tx, bool_t odd) {
	tx &= 0x1;
	odd &= 0x1;
	return (tx << 2) | (odd << 1);
}

#endif /* SRC_DRIV_USBH_USBH_PSTRUCTS_H_ */
