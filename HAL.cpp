#include <iostream>
#include <memory>
#include <string>
#include <map>

// ---------------- Logger (Singleton) ----------------
class Logger {
private:
    Logger() {}
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& msg) {
        std::cout << "[INFO] " << msg << std::endl;
    }

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
};

// ---------------- Hardware Interface ----------------
class IHardwareDevice {
public:
    virtual void powerOn() = 0;
    virtual void powerOff() = 0;
    virtual std::string status() = 0;
    virtual ~IHardwareDevice() {}
};

// ---------------- Concrete Devices ----------------
class Motor : public IHardwareDevice {
public:
    void powerOn() override {
        Logger::getInstance().log("Powering on Motor...");
    }
    void powerOff() override {
        Logger::getInstance().log("Powering off Motor...");
    }
    std::string status() override {
        return "Motor operational.";
    }
};

class LED : public IHardwareDevice {
public:
    void powerOn() override {
        Logger::getInstance().log("Powering on LED...");
    }
    void powerOff() override {
        Logger::getInstance().log("Powering off LED...");
    }
    std::string status() override {
        return "LED active.";
    }
};

class RFModule : public IHardwareDevice {
public:
    void powerOn() override {
        Logger::getInstance().log("Powering on RFModule...");
    }
    void powerOff() override {
        Logger::getInstance().log("Powering off RFModule...");
    }
    std::string status() override {
        return "RFModule transmitting.";
    }
};

// ---------------- Factory Pattern ----------------
class DeviceFactory {
public:
    static std::unique_ptr<IHardwareDevice> createDevice(const std::string& type) {
        if (type == "Motor") return std::make_unique<Motor>();
        else if (type == "LED") return std::make_unique<LED>();
        else if (type == "RFModule") return std::make_unique<RFModule>();
        else throw std::invalid_argument("Unknown device type: " + type);
    }
};

// ---------------- Strategy Pattern ----------------
class IControlMode {
public:
    virtual void applyMode(IHardwareDevice& device) = 0;
    virtual std::string name() = 0;
    virtual ~IControlMode() {}
};

class SafeMode : public IControlMode {
public:
    void applyMode(IHardwareDevice& device) override {
        Logger::getInstance().log("Applying Safe mode...");
        device.powerOn();
    }
    std::string name() override { return "Safe"; }
};

class PerformanceMode : public IControlMode {
public:
    void applyMode(IHardwareDevice& device) override {
        Logger::getInstance().log("Applying Performance mode...");
        device.powerOn();
    }
    std::string name() override { return "Performance"; }
};

// ---------------- Main Simulation ----------------
int main() {
    try {
        auto device = DeviceFactory::createDevice("RFModule");
        Logger::getInstance().log("Created RFModule");

        std::unique_ptr<IControlMode> mode = std::make_unique<SafeMode>();
        Logger::getInstance().log("Control mode: " + mode->name());

        mode->applyMode(*device);

        Logger::getInstance().log(device->status());
    } 
    catch (const std::exception& e) {
        Logger::getInstance().log(std::string("Error: ") + e.what());
    }
}

