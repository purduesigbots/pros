#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "kapi.h"
#include "v5_api.h"


// TODO: not vexStub
__thread struct _reent* _REENT;

void vexDisplayPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque, const char* format, ...) {}

void* kmalloc(size_t size) {
	return malloc(size);
}

void kfree(void* toFree) {
	free(toFree);
}

int32_t vexBatteryVoltageGet(void) {
	return 0;
}

double vexBatteryTemperatureGet(void) {
	return 0;
}

int32_t vexBatteryCurrentGet(void) {
	return 0;
}

double vexBatteryCapacityGet(void) {
	return 0;
}

V5_ControllerStatus vexControllerConnectionStatusGet(V5_ControllerId id) {
	return kV5ControllerOffline;
}

int32_t vexControllerGet(V5_ControllerId id, V5_ControllerIndex index) {
	return 0;
}

bool vexControllerTextSet(V5_ControllerId id, uint32_t line, uint32_t col, const char* str) {
	return false;
}

uint32_t vexSystemVersion(void) {
	return 0;
}

uint32_t vexCompetitionStatus(void) {
	return 0;
}

V5_DeviceT vexDeviceGetByIndex(uint32_t index) {
	return kDeviceTypeNoSensor;
}

V5_AdiPortConfiguration vexDeviceAdiPortConfigGet(V5_DeviceT device, uint32_t port) {
	return kAdiPortTypeAnalogIn;
}

void vexDisplayCenteredString(const int32_t nLineNumber, const char* format, ...) {
	return;
}

void vexDisplayForegroundColor(uint32_t col) {
	return;
}

void vexDisplayRectFill(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	return;
}

void vexDisplayCopyRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t* pSrc, int32_t srcStride) {
	return;
}

int32_t vexDeviceGetStatus(V5_DeviceType* buffer) {
	return 0;
}

int32_t vexDeviceAdiValueGet(V5_DeviceT device, uint32_t port) {
	return 0;
}

void vexDeviceAdiPortConfigSet(V5_DeviceT device, uint32_t port, V5_AdiPortConfiguration type) {
	return;
}

void vexDeviceAdiValueSet(V5_DeviceT device, uint32_t port, int32_t value) {
	return;
}

void vexDeviceMotorAbsoluteTargetSet(V5_DeviceT device, double position, int32_t velocity) {
	return;
}

void vexDeviceMotorVelocitySet(V5_DeviceT device, int32_t velocity) {
	return;
}

void vexDeviceMotorRelativeTargetSet(V5_DeviceT device, double position, int32_t velocity) {
	return;
}

void vexDeviceMotorVoltageSet(V5_DeviceT device, int32_t value) {
	return;
}

void vexDeviceMotorVelocityUpdate(V5_DeviceT device, int32_t velocity) {
	return;
}

double vexDeviceMotorTargetGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceMotorVelocityGet(V5_DeviceT device) {
	return 0;
}

double vexDeviceMotorActualVelocityGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceMotorCurrentGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceMotorDirectionGet(V5_DeviceT device) {
	return 0;
}

void vexDeviceMotorCurrentLimitSet(V5_DeviceT device, int32_t value) {
	return;
}

bool vexDeviceMotorOverTempFlagGet(V5_DeviceT device) {
	return false;
}

uint32_t vexDeviceMotorFaultsGet(V5_DeviceT device) {
	return 0;
}

uint32_t vexDeviceMotorFlagsGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceMotorPositionRawGet(V5_DeviceT device, uint32_t* timestamp) {
	return 0;
}

double vexDeviceMotorPositionGet(V5_DeviceT device) {
	return 0;
}

double vexDeviceMotorPowerGet(V5_DeviceT device) {
	return 0;
}

double vexDeviceMotorEfficiencyGet(V5_DeviceT device) {
	return 0;
}

bool vexDeviceMotorCurrentLimitFlagGet(V5_DeviceT device) {
	return false;
}

double vexDeviceMotorTemperatureGet(V5_DeviceT device) {
	return 0;
}

double vexDeviceMotorTorqueGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceMotorVoltageGet(V5_DeviceT device) {
	return 0;
}

void vexDeviceMotorPositionSet(V5_DeviceT device, double position) {}

void vexDeviceMotorPositionReset(V5_DeviceT device) {}

void vexDeviceMotorBrakeModeSet(V5_DeviceT device, V5MotorBrakeMode mode) {}

void vexDeviceMotorEncoderUnitsSet(V5_DeviceT device, V5MotorEncoderUnits units) {
	return;
}

void vexDeviceMotorGearingSet(V5_DeviceT device, V5MotorGearset value) {}

void vexDeviceMotorPositionPidSet(V5_DeviceT device, V5_DeviceMotorPid* pid) {
	return;
}

void vexDeviceMotorVoltageLimitSet(V5_DeviceT device, int32_t value) {
	return;
}

V5MotorBrakeMode vexDeviceMotorBrakeModeGet(V5_DeviceT device) {
	return kV5MotorBrakeModeCoast;
}

void vexDeviceMotorReverseFlagSet(V5_DeviceT device, bool value) {
	return;
}

int32_t vexDeviceMotorCurrentLimitGet(V5_DeviceT device) {
	return 0;
}

V5MotorEncoderUnits vexDeviceMotorEncoderUnitsGet(V5_DeviceT device) {
	return kMotorEncoderDegrees;
}

V5MotorGearset vexDeviceMotorGearingGet(V5_DeviceT device) {
	return kMotorGearSet_36;
}

bool vexDeviceMotorReverseFlagGet(V5_DeviceT device) {
	return false;
}

int32_t vexDeviceMotorVoltageLimitGet(V5_DeviceT device) {
	return 0;
}

void vexDeviceGenericSerialEnable(V5_DeviceT device, int32_t options) {
	return;
}

void vexDeviceGenericSerialBaudrate(V5_DeviceT device, int32_t baudrate) {
	return;
}

void vexDeviceGenericSerialFlush(V5_DeviceT device) {
	return;
}

int32_t vexDeviceGenericSerialReceiveAvail(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceGenericSerialWriteFree(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceGenericSerialPeekChar(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceGenericSerialReadChar(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceGenericSerialReceive(V5_DeviceT device, uint8_t* buffer, int32_t length) {
	return 0;
}

int32_t vexDeviceGenericSerialWriteChar(V5_DeviceT device, uint8_t c) {
	return 0;
}

int32_t vexDeviceGenericSerialTransmit(V5_DeviceT device, uint8_t* buffer, int32_t length) {
	return 0;
}

int32_t vexDeviceVisionObjectCountGet(V5_DeviceT device) {
	return 0;
}

int32_t vexDeviceVisionObjectGet(V5_DeviceT device, uint32_t indexObj, V5_DeviceVisionObject* pObject) {
	return 0;
}

bool vexDeviceVisionSignatureGet(V5_DeviceT device, uint32_t id, V5_DeviceVisionSignature* pSignature) {
	return false;
}

void vexDeviceVisionSignatureSet(V5_DeviceT device, V5_DeviceVisionSignature* pSignature) {
	return;
}

void vexDeviceVisionLedModeSet(V5_DeviceT device, V5VisionLedMode mode) {
	return;
}

void vexDeviceVisionLedColorSet(V5_DeviceT device, V5_DeviceVisionRgb color) {
	return;
}

void vexDeviceVisionBrightnessSet(V5_DeviceT device, uint8_t percent) {
	return;
}

uint8_t vexDeviceVisionBrightnessGet(V5_DeviceT device) {
	return 0;
}

void vexDeviceVisionWhiteBalanceModeSet(V5_DeviceT device, V5VisionWBMode mode) {
	return;
}

V5_DeviceVisionRgb vexDeviceVisionWhiteBalanceGet(V5_DeviceT device) {
	V5_DeviceVisionRgb empty = {0};
	return empty;
}

void vexDeviceVisionWifiModeSet(V5_DeviceT device, V5VisionWifiMode mode) {
	return;
}

int32_t vexSerialReadChar(uint32_t channel) {
	return 0;
}

// TODO: newlib stub
int iprintf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int rtn = vprintf(format, args);
	va_end(args);
	return rtn;
}

void vexDeviceVisionWhiteBalanceSet(V5_DeviceT device, V5_DeviceVisionRgb color) {
	return;
}

int32_t vexSerialWriteFree(uint32_t channel) {
	return 0;
}

int32_t vexSerialWriteBuffer(uint32_t channel, uint8_t* data, uint32_t data_len) {
	return 0;
}

int32_t vexFileRead(char* buf, uint32_t size, uint32_t nItems, FIL* fdp) {
	return 0;
}

int32_t vexFileWrite(char* buf, uint32_t size, uint32_t nItems, FIL* fdp) {
	return 0;
}

void vexFileClose(FIL* fdp) {
	return;
}

int32_t vexFileSize(FIL* fdp) {
	return 0;
}

FRESULT vexFileSeek(FIL* fdp, uint32_t offset, int32_t whence) {
	FRESULT empty = {0};
	return empty;
}

int32_t vexFileTell(FIL* fdp) {
	return 0;
}

FRESULT vexFileMountSD(void) {
	FRESULT empty = {0};
	return empty;
}

FIL* vexFileOpen(const char* filename, const char* mode) {
	return NULL;
}

FIL* vexFileOpenWrite(const char* filename) {
	return NULL;
}

FIL* vexFileOpenCreate(const char* filename) {
	return NULL;
}

void vexBackgroundProcessing(void) {
	return;
}

void vexDisplayString(const int32_t nLineNumber, const char* format, ...) {
	return;
}

bool vexTouchDataGet(V5_TouchStatus* status) {
	return false;
}

stream_buf_t xStreamBufferGenericCreateStatic(size_t xBufferSizeBytes, size_t xTriggerLevelBytes,
                                              int32_t xIsMessageBuffer, uint8_t* const pucStreamBufferStorageArea,
                                              static_stream_buf_s_t* const pxStaticStreamBuffer) {
	return xStreamBufferGenericCreate(xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer);
}

task_t task_create_static(task_fn_t task_code, void* const param, uint32_t priority, const size_t stack_size,
                          const char* const name, task_stack_t* const stack_buffer,
                          static_task_s_t* const task_buffer) {
	return task_create(task_code, param, priority, stack_size, name);
}

uint32_t vexSystemLinkAddrGet(void) {
	return 0;
}

void vAssertCalled(const char* pcFile, unsigned long ulLine) {
	assert(false);
}

queue_t xQueueCreateMutex( const uint8_t ucQueueType ) ;
queue_t xQueueCreateCountingSemaphore( const uint32_t uxMaxCount, const uint32_t uxInitialCount ) ;

queue_t xQueueCreateMutexStatic(const uint8_t ucQueueType, static_queue_s_t* pxStaticQueue) 
{
	return xQueueCreateMutex(ucQueueType);
}

queue_t xQueueCreateCountingSemaphoreStatic(const uint32_t uxMaxCount, const uint32_t uxInitialCount,
                                           static_queue_s_t* pxStaticQueue) 
{
	return xQueueCreateCountingSemaphore(uxMaxCount, uxInitialCount);
}

//TODO: fix this duplicated code
void rtos_initialize() {
	void task_notify_when_deleting_init();
	task_notify_when_deleting_init();
}

unsigned __exidx_start;
unsigned __exidx_end;
