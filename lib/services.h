// services.h
#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>
class Service {
    SC_HANDLE scm = nullptr;
    SC_HANDLE svc = nullptr;

public:
    Service(const std::string& name) {
        scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!scm) throw std::runtime_error("Failed to open SCM");

        svc = OpenServiceA(scm, name.c_str(), SERVICE_STOP);
        if (!svc) {
            CloseServiceHandle(scm);
            throw std::runtime_error("Failed to open service");
        }
    }

    ~Service() {
        if (svc) CloseServiceHandle(svc);
        if (scm) CloseServiceHandle(scm);
    }

    void stop() {
        SERVICE_STATUS status{};
        if (!ControlService(svc, SERVICE_CONTROL_STOP, &status)) {
            throw std::runtime_error("Failed to stop service");
        }
    }
};
