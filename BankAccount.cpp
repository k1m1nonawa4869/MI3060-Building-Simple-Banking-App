// File: BankAccount.cpp
#include "BankAccount.h"
#include <ctime>

static std::string currentTime() {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}

BankAccount::BankAccount(int acctId,
                         const std::string& acctName,
                         const std::string& acctPin,
                         int acctBalance,
                         const std::string& createdTime,
                         const std::string& modifiedTime)
    : id(acctId),
      name(acctName),
      balance(acctBalance),
      pin(acctPin.empty() ? std::string("0000") : acctPin),
      locked(false),
      created(createdTime.empty() ? currentTime() : createdTime),
      modified(modifiedTime.empty() ? currentTime() : modifiedTime)
{}

bool BankAccount::verifyPin(const std::string& inputPin) const {
    return inputPin == pin;
}

void BankAccount::setPin(const std::string& newPin) {
    pin = newPin;
    updateModified();
}

const std::string& BankAccount::getPin() const {
    return pin;
}

bool BankAccount::isLocked() const {
    return locked;
}

void BankAccount::setLocked(bool s) {
    locked = s;
    updateModified();
}

const std::string& BankAccount::getCreated() const {
    return created;
}

const std::string& BankAccount::getModified() const {
    return modified;
}

void BankAccount::updateModified() {
    modified = currentTime();
}