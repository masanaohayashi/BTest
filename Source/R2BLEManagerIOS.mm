/*
  ==============================================================================

    R2BLEManagerIOS.mm
    Created: 23 Jan 2025 12:29:47am
    Author:  ring2

  ==============================================================================
*/
#import "R2BLEManagerIOS.h"
#import <CoreBluetooth/CoreBluetooth.h>

// ImplクラスのObjective-C宣言
@interface R2BLEManagerIOSImpl : NSObject <CBCentralManagerDelegate>
@property (nonatomic, assign) R2BLEManagerIOS *owner;
@property (nonatomic, strong) CBCentralManager *centralManager;

- (instancetype)initWithOwner:(R2BLEManagerIOS *)owner;
- (void)startScanningWithUUID:(const std::string &)uuid;
- (void)stopScanning;
- (BOOL)isBluetoothEnabled;
@end

@implementation R2BLEManagerIOSImpl {
    NSMutableArray<CBPeripheral *> *discoveredDevices;
}

- (instancetype)initWithOwner:(R2BLEManagerIOS *)owner {
    self = [super init];
    if (self) {
        self.owner = owner;
        self.centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
        discoveredDevices = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc {
    [self.centralManager stopScan];
    [super dealloc]; // ARC is disabled (Objective-C Automatic Reference Counting = No) so we need this.
}

- (void)startScanningWithUUID:(const std::string &)uuid {
    if (self.centralManager.state == CBManagerStatePoweredOn) {
        if (!uuid.empty()) {
            CBUUID *serviceUUID = [CBUUID UUIDWithString:[NSString stringWithUTF8String:uuid.c_str()]];
            [self.centralManager scanForPeripheralsWithServices:@[serviceUUID] options:nil];
        } else {
            [self.centralManager scanForPeripheralsWithServices:nil options:nil];
        }
        NSLog(@"Started scanning for BLE devices.");
    } else {
        NSLog(@"Bluetooth is not powered on.");
    }
}

- (void)stopScanning {
    [self.centralManager stopScan];
    NSLog(@"Stopped scanning for BLE devices.");
}

- (BOOL)isBluetoothEnabled {
    return self.centralManager.state == CBManagerStatePoweredOn;
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    // デバイス名の取得優先順位
    NSString *deviceName = advertisementData[CBAdvertisementDataLocalNameKey] ?: peripheral.name ?: @"Unknown Device";
    
    // サービスUUIDの取得
    NSArray<CBUUID *> *serviceUUIDs = advertisementData[CBAdvertisementDataServiceUUIDsKey];
    NSString *serviceUUIDString = @"";
    if (serviceUUIDs.count > 0) {
        serviceUUIDString = serviceUUIDs.firstObject.UUIDString; // 最初のUUIDを取得
    }

    //NSLog(@"Discovered device: %@, Identifier: %@, Service UUID: %@, Advertisement Data: %@", deviceName, peripheral.identifier.UUIDString, serviceUUIDString, advertisementData);

    // JUCEのデバイスリストに追加
    juce::String juceDeviceName = juce::String::fromUTF8(deviceName.UTF8String);
    juce::String juceServiceUUID = juce::String::fromUTF8(serviceUUIDString.UTF8String);

    self.owner->addOrUpdateDevice(juce::String(peripheral.identifier.UUIDString.UTF8String), juceServiceUUID, juceDeviceName);
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    switch (central.state) {
        case CBManagerStatePoweredOn:
            NSLog(@"Bluetooth is powered on. Starting scan...");
            [self startScanningWithUUID:""]; // UUIDを指定しない場合、すべてのデバイスをスキャン
            break;
        case CBManagerStatePoweredOff:
            NSLog(@"Bluetooth is powered off.");
            break;
        case CBManagerStateUnauthorized:
            NSLog(@"Bluetooth usage is not authorized.");
            break;
        case CBManagerStateUnsupported:
            NSLog(@"Bluetooth LE is not supported on this device.");
            break;
        case CBManagerStateResetting:
            NSLog(@"Bluetooth is resetting.");
            break;
        case CBManagerStateUnknown:
            NSLog(@"Bluetooth state is unknown.");
            break;
    }
}
@end

// R2BLEManagerIOS::Implクラスの実装
class R2BLEManagerIOS::Impl {
public:
    Impl(R2BLEManagerIOS* owner) : objImpl([[R2BLEManagerIOSImpl alloc] initWithOwner:owner]) {}
    ~Impl() { [objImpl release]; }

    void startScanningWithUUID(const std::string& uuid) {
        if (objImpl.centralManager.state == CBManagerStatePoweredOn) { // `self`を`objImpl`に変更
            if (!uuid.empty()) {
                CBUUID *serviceUUID = [CBUUID UUIDWithString:[NSString stringWithUTF8String:uuid.c_str()]];
                [objImpl.centralManager scanForPeripheralsWithServices:@[serviceUUID] options:nil];
                NSLog(@"Scanning started with UUID: %@", serviceUUID);
            } else {
                [objImpl.centralManager scanForPeripheralsWithServices:nil options:nil];
                NSLog(@"Scanning started for all devices.");
            }
        } else {
            NSLog(@"Bluetooth is not powered on.");
        }
    }

    void stopScanning() {
        [objImpl stopScanning];
    }

    bool isBluetoothEnabled() const {
        return [objImpl isBluetoothEnabled];
    }

private:
    R2BLEManagerIOSImpl* objImpl;
};

// R2BLEManagerIOSクラスのC++部分
R2BLEManagerIOS::R2BLEManagerIOS() : impl(std::make_unique<Impl>(this)) {}
R2BLEManagerIOS::~R2BLEManagerIOS() = default;

void R2BLEManagerIOS::startScanningWithUUID(const std::string &uuid) {
    impl->startScanningWithUUID(uuid);
}

void R2BLEManagerIOS::stopScanning() {
    impl->stopScanning();
}

bool R2BLEManagerIOS::isBluetoothEnabled() const {
    return impl->isBluetoothEnabled();
}

