/**
  *
  * WMI
  * @author Thomas Sparber (2016)
  *
 **/

#ifndef WMICLASSES_HPP
#define WMICLASSES_HPP

#include <string>

#include "wmi.hpp"
#include "wmiresult.hpp"

namespace Wmi {
    struct Win32_ComputerSystemProduct {
        Win32_ComputerSystemProduct() :
                Caption(),
                Description(),
                IdentifyingNumber(),
                Name(),
                UUID(),
                Vendor(),
                Version() {}
        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "IdentifyingNumber", (*this).IdentifyingNumber);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "UUID", (*this).UUID);
            result.extract(index, "Vendor", (*this).Vendor);
            result.extract(index, "Version", (*this).Version);
        }

        static std::string getWmiClassName() {
            return "Win32_ComputerSystemProduct";
        }

        std::string Caption;
        std::string Description;
        std::string IdentifyingNumber;
        std::string Name;
        std::string UUID;
        std::string Vendor;
        std::string Version;
    };

    struct Win32_ComputerSystem {

        Win32_ComputerSystem() :
                AdminPasswordStatus(), AutomaticManagedPagefile(), AutomaticResetBootOption(),
                AutomaticResetCapability(),
                BootOptionOnLimit(), BootOptionOnWatchDog(), BootROMSupported(), BootupState(), Caption(),
                ChassisBootupState(),
                CreationClassName(), CurrentTimeZone(), DaylightInEffect(), Description(), DNSHostName(), Domain(),
                DomainRole(),
                EnableDaylightSavingsTime(), FrontPanelResetStatus(), InfraredSupported(), InitialLoadInfo(),
                InstallDate(),
                KeyboardPasswordStatus(), LastLoadInfo(), Manufacturer(), Model(), Name(), NameFormat(),
                NetworkServerModeEnabled(),
                NumberOfLogicalProcessors(), NumberOfProcessors(), OEMLogoBitmap(), OEMStringArray(), PartOfDomain(),
                PauseAfterReset(), PCSystemType(), PowerManagementCapabilities(), PowerManagementSupported(),
                PowerOnPasswordStatus(), PowerState(), PowerSupplyState(), PrimaryOwnerContact(), PrimaryOwnerName(),
                ResetCapability(), ResetCount(), ResetLimit(), Roles(), Status(), SupportContactDescription(),
                SystemStartupDelay(),
                SystemStartupOptions(), SystemStartupSetting(), SystemType(), ThermalState(), TotalPhysicalMemory(),
                UserName(),
                WakeUpType(), Workgroup() {}

        void setProperties(const WmiResult &result, std::size_t index) {

            result.extract(index, "AdminPasswordStatus", (*this).AdminPasswordStatus);
            result.extract(index, "AutomaticManagedPagefile", (*this).AutomaticManagedPagefile);
            result.extract(index, "AutomaticResetBootOption", (*this).AutomaticResetBootOption);
            result.extract(index, "AutomaticResetCapability", (*this).AutomaticResetCapability);
            result.extract(index, "BootOptionOnLimit", (*this).BootOptionOnLimit);
            result.extract(index, "BootOptionOnWatchDog", (*this).BootOptionOnWatchDog);
            result.extract(index, "BootROMSupported", (*this).BootROMSupported);
            result.extract(index, "BootupState", (*this).BootupState);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "ChassisBootupState", (*this).ChassisBootupState);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "CurrentTimeZone", (*this).CurrentTimeZone);
            result.extract(index, "DaylightInEffect", (*this).DaylightInEffect);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DNSHostName", (*this).DNSHostName);
            result.extract(index, "Domain", (*this).Domain);
            result.extract(index, "DomainRole", (*this).DomainRole);
            result.extract(index, "EnableDaylightSavingsTime", (*this).EnableDaylightSavingsTime);
            result.extract(index, "FrontPanelResetStatus", (*this).FrontPanelResetStatus);
            result.extract(index, "InfraredSupported", (*this).InfraredSupported);
            result.extract(index, "InitialLoadInfo", (*this).InitialLoadInfo);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "KeyboardPasswordStatus", (*this).KeyboardPasswordStatus);
            result.extract(index, "LastLoadInfo", (*this).LastLoadInfo);
            result.extract(index, "Manufacturer", (*this).Manufacturer);
            result.extract(index, "Model", (*this).Model);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "NameFormat", (*this).NameFormat);
            result.extract(index, "NetworkServerModeEnabled", (*this).NetworkServerModeEnabled);
            result.extract(index, "NumberOfLogicalProcessors", (*this).NumberOfLogicalProcessors);
            result.extract(index, "NumberOfProcessors", (*this).NumberOfProcessors);
            result.extract(index, "OEMLogoBitmap", (*this).OEMLogoBitmap);
            result.extract(index, "OEMStringArray", (*this).OEMStringArray);
            result.extract(index, "PartOfDomain", (*this).PartOfDomain);
            result.extract(index, "PauseAfterReset", (*this).PauseAfterReset);
            result.extract(index, "PCSystemType", (*this).PCSystemType);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "PowerOnPasswordStatus", (*this).PowerOnPasswordStatus);
            result.extract(index, "PowerState", (*this).PowerState);
            result.extract(index, "PowerSupplyState", (*this).PowerSupplyState);
            result.extract(index, "PrimaryOwnerContact", (*this).PrimaryOwnerContact);
            result.extract(index, "PrimaryOwnerName", (*this).PrimaryOwnerName);
            result.extract(index, "ResetCapability", (*this).ResetCapability);
            result.extract(index, "ResetCount", (*this).ResetCount);
            result.extract(index, "ResetLimit", (*this).ResetLimit);
            result.extract(index, "Roles", (*this).Roles);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "SupportContactDescription", (*this).SupportContactDescription);
            result.extract(index, "SystemStartupDelay", (*this).SystemStartupDelay);
            result.extract(index, "SystemStartupOptions", (*this).SystemStartupOptions);
            result.extract(index, "SystemStartupSetting", (*this).SystemStartupSetting);
            result.extract(index, "SystemType", (*this).SystemType);
            result.extract(index, "ThermalState", (*this).ThermalState);
            result.extract(index, "TotalPhysicalMemory", (*this).TotalPhysicalMemory);
            result.extract(index, "UserName", (*this).UserName);
            result.extract(index, "WakeUpType", (*this).WakeUpType);
            result.extract(index, "Workgroup", (*this).Workgroup);
        }

        static std::string getWmiClassName() {
            return "Win32_ComputerSystem";
        }

        int AdminPasswordStatus;
        bool AutomaticManagedPagefile;
        bool AutomaticResetBootOption;
        bool AutomaticResetCapability;
        std::string BootOptionOnLimit;
        std::string BootOptionOnWatchDog;
        bool BootROMSupported;
        std::string BootupState;
        std::string Caption;
        int ChassisBootupState;
        std::string CreationClassName;
        int CurrentTimeZone;
        bool DaylightInEffect;
        std::string Description;
        std::string DNSHostName;
        std::string Domain;
        int DomainRole;
        bool EnableDaylightSavingsTime;
        int FrontPanelResetStatus;
        bool InfraredSupported;
        std::string InitialLoadInfo;
        std::string InstallDate;
        int KeyboardPasswordStatus;
        std::string LastLoadInfo;
        std::string Manufacturer;
        std::string Model;
        std::string Name;
        std::string NameFormat;
        bool NetworkServerModeEnabled;
        int NumberOfLogicalProcessors;
        int NumberOfProcessors;
        std::string OEMLogoBitmap;
        std::string OEMStringArray;
        bool PartOfDomain;
        int PauseAfterReset;
        int PCSystemType;
        std::string PowerManagementCapabilities;
        std::string PowerManagementSupported;
        int PowerOnPasswordStatus;
        int PowerState;
        int PowerSupplyState;
        std::string PrimaryOwnerContact;
        std::string PrimaryOwnerName;
        int ResetCapability;
        int ResetCount;
        int ResetLimit;
        std::vector<std::string> Roles;
        std::string Status;
        std::string SupportContactDescription;
        std::string SystemStartupDelay;
        std::string SystemStartupOptions;
        std::string SystemStartupSetting;
        std::string SystemType;
        int ThermalState;
        uint64_t TotalPhysicalMemory;
        std::string UserName;
        int WakeUpType;
        std::string Workgroup;

    }; //end struct Win32_ComputerSystem

    struct Win32_ParallelPort {

        Win32_ParallelPort() :
                Availability(), Capabilities(), CapabilityDescriptions(), Caption(), ConfigManagerErrorCode(),
                ConfigManagerUserConfig(), CreationClassName(), Description(), DeviceID(), DMASupport(), ErrorCleared(),
                ErrorDescription(), InstallDate(), LastErrorCode(), MaxNumberControlled(), Name(), OSAutoDiscovered(),
                PNPDeviceID(), PowerManagementCapabilities(), PowerManagementSupported(), ProtocolSupported(), Status(),
                StatusInfo(), SystemCreationClassName(), SystemName(), TimeOfLastReset() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            //vscode was a great help with search(.+?),
            //and replace result.extract(index, "$1", (*this).$1);
            result.extract(index, "Availability", (*this).Availability);
            result.extract(index, "Capabilities", (*this).Capabilities);
            result.extract(index, "CapabilityDescriptions", (*this).CapabilityDescriptions);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "ConfigManagerErrorCode", (*this).ConfigManagerErrorCode);
            result.extract(index, "ConfigManagerUserConfig", (*this).ConfigManagerUserConfig);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DeviceID", (*this).DeviceID);
            result.extract(index, "DMASupport", (*this).DMASupport);
            result.extract(index, "ErrorCleared", (*this).ErrorCleared);
            result.extract(index, "ErrorDescription", (*this).ErrorDescription);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "LastErrorCode", (*this).LastErrorCode);
            result.extract(index, "MaxNumberControlled", (*this).MaxNumberControlled);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "OSAutoDiscovered", (*this).OSAutoDiscovered);
            result.extract(index, "PNPDeviceID", (*this).PNPDeviceID);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "ProtocolSupported", (*this).ProtocolSupported);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "StatusInfo", (*this).StatusInfo);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "TimeOfLastReset", (*this).TimeOfLastReset);

        }

        static std::string getWmiClassName() {
            return "Win32_ParallelPort";
        }

        int Availability;
        std::string Capabilities;
        std::string CapabilityDescriptions;
        std::string Caption;
        int ConfigManagerErrorCode;
        bool ConfigManagerUserConfig;
        std::string CreationClassName;
        std::string Description;
        std::string DeviceID;
        bool DMASupport;
        std::string ErrorCleared;
        std::string ErrorDescription;
        std::string InstallDate;
        int LastErrorCode;
        int MaxNumberControlled;
        std::string Name;
        bool OSAutoDiscovered;
        std::string PNPDeviceID;
        std::string PowerManagementCapabilities;
        bool PowerManagementSupported;
        int ProtocolSupported;
        std::string Status;
        std::string StatusInfo;
        std::string SystemCreationClassName;
        std::string SystemName;
        std::string TimeOfLastReset;

    }; //end class Win32_ParallelPort

    struct Win32_PhysicalMedia {
        Win32_PhysicalMedia() :
                Caption(), Description(), InstallDate(), Name(), Status(), CreationClassName(),
                Manufacturer(), Model(), SKU(), SerialNumber(), Tag(), Version(), PartNumber(),
                OtherIdentifyingInfo(), PoweredOn(), Removable(), Replaceable(), HotSwappable(),
                Capacity(), MediaType(), MediaDescription(), WriteProtectOn(), CleanerMedia() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Manufacturer", (*this).Manufacturer);
            result.extract(index, "Model", (*this).Model);
            result.extract(index, "SKU", (*this).SKU);
            result.extract(index, "SerialNumber", (*this).SerialNumber);
            result.extract(index, "Tag", (*this).Tag);
            result.extract(index, "Version", (*this).Version);
            result.extract(index, "PartNumber", (*this).PartNumber);
            result.extract(index, "OtherIdentifyingInfo", (*this).OtherIdentifyingInfo);
            result.extract(index, "PoweredOn", (*this).Removable);
            result.extract(index, "Replaceable", (*this).Replaceable);
            result.extract(index, "HotSwappable", (*this).HotSwappable);
            result.extract(index, "Capacity", (*this).Capacity);
            result.extract(index, "MediaType", (*this).MediaType);
            result.extract(index, "MediaDescription", (*this).MediaDescription);
            result.extract(index, "WriteProtectOn", (*this).WriteProtectOn);
            result.extract(index, "CleanerMedia", (*this).CleanerMedia);
        }

        static std::string getWmiClassName() {
            return "Win32_PhysicalMedia";
        }

        std::string Caption;
        std::string Description;
        std::string InstallDate;
        std::string Name;
        std::string Status;
        std::string CreationClassName;
        std::string Manufacturer;
        std::string Model;
        std::string SKU;
        std::string SerialNumber;
        std::string Tag;
        std::string Version;
        std::string PartNumber;
        std::string OtherIdentifyingInfo;
        bool PoweredOn;
        bool Removable;
        bool Replaceable;
        bool HotSwappable;
        std::string Capacity;
        int MediaType;
        std::string MediaDescription;
        bool WriteProtectOn;
        bool CleanerMedia;

    }; //end class Win32_PhysicalMedia

    struct Win32_Processor {
        Win32_Processor() :
                AddressWidth(), Architecture(), AssetTag(), Availability(), Caption(), Characteristics(),
                ConfigManagerErrorCode(),
                ConfigManagerUserConfig(), CpuStatus(), CreationClassName(), CurrentClockSpeed(), CurrentVoltage(),
                DataWidth(),
                Description(), DeviceID(), ErrorCleared(), ErrorDescription(), ExtClock(), Family(), InstallDate(),
                L2CacheSize(), L2CacheSpeed(), L3CacheSize(), L3CacheSpeed(), LastErrorCode(), Level(),
                LoadPercentage(),
                Manufacturer(), MaxClockSpeed(), Name(), NumberOfCores(), NumberOfEnabledCore(),
                NumberOfLogicalProcessors(),
                OtherFamilyDescription(), PartNumber(), PNPDeviceID(), PowerManagementCapabilities(),
                PowerManagementSupported(), ProcessorId(),
                ProcessorType(), Revision(), Role(), SecondLevelAddressTranslationExtensions(), SerialNumber(),
                SocketDesignation(),
                Status(), StatusInfo(), Stepping(), SystemCreationClassName(), SystemName(), ThreadCount(), UniqueId(),
                UpgradeMethod(), Version(), VirtualizationFirmwareEnabled(), VMMonitorModeExtensions(), VoltageCaps() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            //vscode was a great help with search(.+?),
            //and replace result.extract(index, "$1", (*this).$1);
            result.extract(index, "AddressWidth", (*this).AddressWidth);
            result.extract(index, "Architecture", (*this).Architecture);
            result.extract(index, "AssetTag", (*this).AssetTag);
            result.extract(index, "Availability", (*this).Availability);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "Characteristics", (*this).Characteristics);
            result.extract(index, "ConfigManagerErrorCode", (*this).ConfigManagerErrorCode);
            result.extract(index, "ConfigManagerUserConfig", (*this).ConfigManagerUserConfig);
            result.extract(index, "CpuStatus", (*this).CpuStatus);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "CurrentClockSpeed", (*this).CurrentClockSpeed);
            result.extract(index, "CurrentVoltage", (*this).CurrentVoltage);
            result.extract(index, "DataWidth", (*this).DataWidth);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DeviceID", (*this).DeviceID);
            result.extract(index, "ErrorCleared", (*this).ErrorCleared);
            result.extract(index, "ErrorDescription", (*this).ErrorDescription);
            result.extract(index, "ExtClock", (*this).ExtClock);
            result.extract(index, "Family", (*this).Family);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "L2CacheSize", (*this).L2CacheSize);
            result.extract(index, "L2CacheSpeed", (*this).L2CacheSpeed);
            result.extract(index, "L3CacheSize", (*this).L3CacheSize);
            result.extract(index, "L3CacheSpeed", (*this).L3CacheSpeed);
            result.extract(index, "LastErrorCode", (*this).LastErrorCode);
            result.extract(index, "Level", (*this).Level);
            result.extract(index, "LoadPercentage", (*this).LoadPercentage);
            result.extract(index, "Manufacturer", (*this).Manufacturer);
            result.extract(index, "MaxClockSpeed", (*this).MaxClockSpeed);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "NumberOfCores", (*this).NumberOfCores);
            result.extract(index, "NumberOfEnabledCore", (*this).NumberOfEnabledCore);
            result.extract(index, "NumberOfLogicalProcessors", (*this).NumberOfLogicalProcessors);
            result.extract(index, "OtherFamilyDescription", (*this).OtherFamilyDescription);
            result.extract(index, "PartNumber", (*this).PartNumber);
            result.extract(index, "PNPDeviceID", (*this).PNPDeviceID);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "ProcessorId", (*this).ProcessorId);
            result.extract(index, "ProcessorType", (*this).ProcessorType);
            result.extract(index, "Revision", (*this).Revision);
            result.extract(index, "SecondLevelAddressTranslationExtensions",
                           (*this).SecondLevelAddressTranslationExtensions);
            result.extract(index, "SerialNumber", (*this).SerialNumber);
            result.extract(index, "SocketDesignation", (*this).SocketDesignation);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "StatusInfo", (*this).StatusInfo);
            result.extract(index, "Stepping", (*this).Stepping);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "ThreadCount", (*this).ThreadCount);
            result.extract(index, "UniqueId", (*this).UniqueId);
            result.extract(index, "UpgradeMethod", (*this).UpgradeMethod);
            result.extract(index, "Version", (*this).Version);
            result.extract(index, "VirtualizationFirmwareEnabled", (*this).VirtualizationFirmwareEnabled);
            result.extract(index, "VMMonitorModeExtensions", (*this).VMMonitorModeExtensions);
            result.extract(index, "VoltageCaps", (*this).VoltageCaps);
        }

        static std::string getWmiClassName() {
            return "Win32_Processor";
        }

        int AddressWidth;
        int Architecture;
        std::string AssetTag;
        int Availability;
        std::string Caption;
        std::string Characteristics;
        int ConfigManagerErrorCode;
        bool ConfigManagerUserConfig;
        int CpuStatus;
        std::string CreationClassName;
        int CurrentClockSpeed;
        int CurrentVoltage;
        int DataWidth;
        std::string Description;
        std::string DeviceID;
        std::string ErrorCleared;
        std::string ErrorDescription;
        int ExtClock;
        int Family;
        std::string InstallDate;
        int L2CacheSize;
        int L2CacheSpeed;
        int L3CacheSize;
        int L3CacheSpeed;
        int LastErrorCode;
        int Level;
        int LoadPercentage;
        std::string Manufacturer;
        int MaxClockSpeed;
        std::string Name;
        int NumberOfCores;
        int NumberOfEnabledCore;
        int NumberOfLogicalProcessors;
        std::string OtherFamilyDescription;
        std::string PartNumber;
        std::string PNPDeviceID;
        std::string PowerManagementCapabilities;
        bool PowerManagementSupported;
        std::string ProcessorId;
        int ProcessorType;
        int Revision;
        std::string Role;
        bool SecondLevelAddressTranslationExtensions;
        std::string SerialNumber;
        std::string SocketDesignation;
        std::string Status;
        std::string StatusInfo;
        std::string Stepping;
        std::string SystemCreationClassName;
        std::string SystemName;
        std::string ThreadCount;
        std::string UniqueId;
        int UpgradeMethod;
        std::string Version;
        bool VirtualizationFirmwareEnabled;
        bool VMMonitorModeExtensions;
        int VoltageCaps;

    }; //end class Win32_Processor

    struct Win32_Service {

        Win32_Service() :
                AcceptPause(), AcceptStop(), Caption(), CheckPoint(), CreationClassName(), Description(),
                DesktopInteract(), DisplayName(), ErrorControl(), ExitCode(), InstallDate(), Name(), PathName(),
                ProcessId(), ServiceSpecificExitCode(), ServiceType(), Started(), StartMode(), StartName(), State(),
                Status(), SystemCreationClassName(), SystemName(), TagId(), WaitHint() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "AcceptPause", (*this).AcceptPause);
            result.extract(index, "AcceptStop", (*this).AcceptStop);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "CheckPoint", (*this).CheckPoint);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DesktopInteract", (*this).DesktopInteract);
            result.extract(index, "DisplayName", (*this).DisplayName);
            result.extract(index, "ErrorControl", (*this).ErrorControl);
            result.extract(index, "ExitCode", (*this).ExitCode);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "PathName", (*this).PathName);
            result.extract(index, "ProcessId", (*this).ProcessId);
            result.extract(index, "ServiceSpecificExitCode", (*this).ServiceSpecificExitCode);
            result.extract(index, "ServiceType", (*this).ServiceType);
            result.extract(index, "Started", (*this).Started);
            result.extract(index, "StartMode", (*this).StartMode);
            result.extract(index, "StartName", (*this).StartName);
            result.extract(index, "State", (*this).State);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "TagId", (*this).TagId);
            result.extract(index, "WaitHint", (*this).WaitHint);
        }

        static std::string getWmiClassName() {
            return "Win32_Service";
        }

        bool AcceptPause;
        bool AcceptStop;
        std::string Caption;
        int CheckPoint;
        std::string CreationClassName;
        std::string Description;
        bool DesktopInteract;
        std::string DisplayName;
        std::string ErrorControl;
        int ExitCode;
        std::string InstallDate;
        std::string Name;
        std::string PathName;
        int ProcessId;
        int ServiceSpecificExitCode;
        std::string ServiceType;
        bool Started;
        std::string StartMode;
        std::string StartName;
        std::string State;
        std::string Status;
        std::string SystemCreationClassName;
        std::string SystemName;
        int TagId;
        int WaitHint;

    }; //end Win32_Service

    struct Win32_SerialPort {

        Win32_SerialPort() :
                Availability(), Binary(), Capabilities(), CapabilityDescriptions(), Caption(),
                ConfigManagerErrorCode(), ConfigManagerUserConfig(), CreationClassName(), Description(),
                DeviceID(), ErrorCleared(), ErrorDescription(), InstallDate(), LastErrorCode(), MaxBaudRate(),
                MaximumInputBufferSize(), MaximumOutputBufferSize(), MaxNumberControlled(), Name(),
                OSAutoDiscovered(), PNPDeviceID(), PowerManagementCapabilities(), PowerManagementSupported(),
                ProtocolSupported(), ProviderType(), SettableBaudRate(), SettableDataBits(),
                SettableFlowControl(), SettableParity(), SettableParityCheck(), SettableRLSD(),
                SettableStopBits(), Status(), StatusInfo(), Supports16BitMode(), SupportsDTRDSR(),
                SupportsElapsedTimeouts(), SupportsIntTimeouts(), SupportsParityCheck(), SupportsRLSD(),
                SupportsRTSCTS(), SupportsSpecialCharacters(), SupportsXOnXOff(), SupportsXOnXOffSet(),
                SystemCreationClassName(), SystemName(), TimeOfLastReset() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "Availability", (*this).Availability);
            result.extract(index, "Binary", (*this).Binary);
            result.extract(index, "Capabilities", (*this).Capabilities);
            result.extract(index, "CapabilityDescriptions", (*this).CapabilityDescriptions);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "ConfigManagerErrorCode", (*this).ConfigManagerErrorCode);
            result.extract(index, "ConfigManagerUserConfig", (*this).ConfigManagerUserConfig);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DeviceID", (*this).DeviceID);
            result.extract(index, "ErrorCleared", (*this).ErrorCleared);
            result.extract(index, "ErrorDescription", (*this).ErrorDescription);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "LastErrorCode", (*this).LastErrorCode);
            result.extract(index, "MaxBaudRate", (*this).MaxBaudRate);
            result.extract(index, "MaximumInputBufferSize", (*this).MaximumInputBufferSize);
            result.extract(index, "MaximumOutputBufferSize", (*this).MaximumOutputBufferSize);
            result.extract(index, "MaxNumberControlled", (*this).MaxNumberControlled);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "OSAutoDiscovered", (*this).OSAutoDiscovered);
            result.extract(index, "PNPDeviceID", (*this).PNPDeviceID);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "ProtocolSupported", (*this).ProtocolSupported);
            result.extract(index, "ProviderType", (*this).ProviderType);
            result.extract(index, "SettableBaudRate", (*this).SettableBaudRate);
            result.extract(index, "SettableDataBits", (*this).SettableDataBits);
            result.extract(index, "SettableFlowControl", (*this).SettableFlowControl);
            result.extract(index, "SettableParity", (*this).SettableParity);
            result.extract(index, "SettableParityCheck", (*this).SettableParityCheck);
            result.extract(index, "SettableRLSD", (*this).SettableRLSD);
            result.extract(index, "SettableStopBits", (*this).SettableStopBits);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "StatusInfo", (*this).StatusInfo);
            result.extract(index, "Supports16BitMode", (*this).Supports16BitMode);
            result.extract(index, "SupportsDTRDSR", (*this).SupportsDTRDSR);
            result.extract(index, "SupportsElapsedTimeouts", (*this).SupportsElapsedTimeouts);
            result.extract(index, "SupportsIntTimeouts", (*this).SupportsIntTimeouts);
            result.extract(index, "SupportsParityCheck", (*this).SupportsParityCheck);
            result.extract(index, "SupportsRLSD", (*this).SupportsRLSD);
            result.extract(index, "SupportsRTSCTS", (*this).SupportsRTSCTS);
            result.extract(index, "SupportsSpecialCharacters", (*this).SupportsSpecialCharacters);
            result.extract(index, "SupportsXOnXOff", (*this).SupportsXOnXOff);
            result.extract(index, "SupportsXOnXOffSet", (*this).SupportsXOnXOffSet);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "TimeOfLastReset", (*this).TimeOfLastReset);
        }

        static std::string getWmiClassName() {
            return "Win32_SerialPort";
        }

        int Availability;
        bool Binary;
        std::string Capabilities;
        std::string CapabilityDescriptions;
        std::string Caption;
        int ConfigManagerErrorCode;
        bool ConfigManagerUserConfig;
        std::string CreationClassName;
        std::string Description;
        std::string DeviceID;
        std::string ErrorCleared;
        std::string ErrorDescription;
        std::string InstallDate;
        int LastErrorCode;
        uint64_t MaxBaudRate;
        uint64_t MaximumInputBufferSize;
        uint64_t MaximumOutputBufferSize;
        int MaxNumberControlled;
        std::string Name;
        bool OSAutoDiscovered;
        std::string PNPDeviceID;
        std::string PowerManagementCapabilities;
        bool PowerManagementSupported;
        int ProtocolSupported;
        std::string ProviderType;
        std::string SettableBaudRate;
        std::string SettableDataBits;
        std::string SettableFlowControl;
        std::string SettableParity;
        std::string SettableParityCheck;
        std::string SettableRLSD;
        std::string SettableStopBits;
        std::string Status;
        std::string StatusInfo;
        bool Supports16BitMode;
        bool SupportsDTRDSR;
        bool SupportsElapsedTimeouts;
        bool SupportsIntTimeouts;
        bool SupportsParityCheck;
        bool SupportsRLSD;
        bool SupportsRTSCTS;
        bool SupportsSpecialCharacters;
        bool SupportsXOnXOff;
        bool SupportsXOnXOffSet;
        std::string SystemCreationClassName;
        std::string SystemName;
        std::string TimeOfLastReset;

    }; //end Win32_SerialPort

    struct SoftwareLicensingService {
        SoftwareLicensingService() :
                ClientMachineID(),
                DiscoveredKeyManagementServiceMachineIpAddress(),
                DiscoveredKeyManagementServiceMachineName(),
                DiscoveredKeyManagementServiceMachinePort(),
                IsKeyManagementServiceMachine(),
                KeyManagementServiceCurrentCount(),
                KeyManagementServiceDnsPublishing(),
                KeyManagementServiceFailedRequests(),
                KeyManagementServiceHostCaching(),
                KeyManagementServiceLicensedRequests(),
                KeyManagementServiceListeningPort(),
                KeyManagementServiceLookupDomain(),
                KeyManagementServiceLowPriority(),
                KeyManagementServiceMachine(),
                KeyManagementServiceNonGenuineGraceRequests(),
                KeyManagementServiceNotificationRequests(),
                KeyManagementServiceOOBGraceRequests(),
                KeyManagementServiceOOTGraceRequests(),
                KeyManagementServicePort(),
                KeyManagementServiceProductKeyID(),
                KeyManagementServiceTotalRequests(),
                KeyManagementServiceUnlicensedRequests(),
                OA2xBiosMarkerMinorVersion(),
                OA2xBiosMarkerStatus(),
                OA3xOriginalProductKey(),
                OA3xOriginalProductKeyDescription(),
                OA3xOriginalProductKeyPkPn(),
                PolicyCacheRefreshRequired(),
                RemainingWindowsReArmCount(),
                RequiredClientCount(),
                TokenActivationAdditionalInfo(),
                TokenActivationCertificateThumbprint(),
                TokenActivationGrantNumber(),
                TokenActivationILID(),
                TokenActivationILVID(),
                Version(),
                VLActivationInterval(),
                VLRenewalInterval() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "ClientMachineID", (*this).ClientMachineID);
            result.extract(index, "DiscoveredKeyManagementServiceMachineIpAddress",
                           (*this).DiscoveredKeyManagementServiceMachineIpAddress);
            result.extract(index, "DiscoveredKeyManagementServiceMachineName",
                           (*this).DiscoveredKeyManagementServiceMachineName);
            result.extract(index, "DiscoveredKeyManagementServiceMachinePort",
                           (*this).DiscoveredKeyManagementServiceMachinePort);
            result.extract(index, "IsKeyManagementServiceMachine", (*this).IsKeyManagementServiceMachine);
            result.extract(index, "KeyManagementServiceCurrentCount", (*this).KeyManagementServiceCurrentCount);
            result.extract(index, "KeyManagementServiceDnsPublishing", (*this).KeyManagementServiceDnsPublishing);
            result.extract(index, "KeyManagementServiceFailedRequests", (*this).KeyManagementServiceFailedRequests);
            result.extract(index, "KeyManagementServiceHostCaching", (*this).KeyManagementServiceHostCaching);
            result.extract(index, "KeyManagementServiceLicensedRequests", (*this).KeyManagementServiceLicensedRequests);
            result.extract(index, "KeyManagementServiceListeningPort", (*this).KeyManagementServiceListeningPort);
            result.extract(index, "KeyManagementServiceLookupDomain", (*this).KeyManagementServiceLookupDomain);
            result.extract(index, "KeyManagementServiceLowPriority", (*this).KeyManagementServiceLowPriority);
            result.extract(index, "KeyManagementServiceMachine", (*this).KeyManagementServiceMachine);
            result.extract(index, "KeyManagementServiceNonGenuineGraceRequests",
                           (*this).KeyManagementServiceNonGenuineGraceRequests);
            result.extract(index, "KeyManagementServiceNotificationRequests",
                           (*this).KeyManagementServiceNotificationRequests);
            result.extract(index, "KeyManagementServiceOOBGraceRequests", (*this).KeyManagementServiceOOBGraceRequests);
            result.extract(index, "KeyManagementServiceOOTGraceRequests", (*this).KeyManagementServiceOOTGraceRequests);
            result.extract(index, "KeyManagementServicePort", (*this).KeyManagementServicePort);
            result.extract(index, "KeyManagementServiceProductKeyID", (*this).KeyManagementServiceProductKeyID);
            result.extract(index, "KeyManagementServiceTotalRequests", (*this).KeyManagementServiceTotalRequests);
            result.extract(index, "KeyManagementServiceUnlicensedRequests",
                           (*this).KeyManagementServiceUnlicensedRequests);
            result.extract(index, "OA2xBiosMarkerMinorVersion", (*this).OA2xBiosMarkerMinorVersion);
            result.extract(index, "OA2xBiosMarkerStatus", (*this).OA2xBiosMarkerStatus);
            result.extract(index, "OA3xOriginalProductKey", (*this).OA3xOriginalProductKey);
            result.extract(index, "OA3xOriginalProductKeyDescription", (*this).OA3xOriginalProductKeyDescription);
            result.extract(index, "OA3xOriginalProductKeyPkPn", (*this).OA3xOriginalProductKeyPkPn);
            result.extract(index, "PolicyCacheRefreshRequired", (*this).PolicyCacheRefreshRequired);
            result.extract(index, "RemainingWindowsReArmCount", (*this).RemainingWindowsReArmCount);
            result.extract(index, "RequiredClientCount", (*this).RequiredClientCount);
            result.extract(index, "TokenActivationAdditionalInfo", (*this).TokenActivationAdditionalInfo);
            result.extract(index, "TokenActivationCertificateThumbprint", (*this).TokenActivationCertificateThumbprint);
            result.extract(index, "TokenActivationGrantNumber", (*this).TokenActivationGrantNumber);
            result.extract(index, "TokenActivationILID", (*this).TokenActivationILID);
            result.extract(index, "TokenActivationILVID", (*this).TokenActivationILVID);
            result.extract(index, "Version", (*this).Version);
            result.extract(index, "VLActivationInterval", (*this).VLActivationInterval);
            result.extract(index, "VLRenewalInterval", (*this).VLRenewalInterval);
        }

        static std::string getWmiClassName() {
            return "SoftwareLicensingService";
        }

        std::string ClientMachineID;
        std::string DiscoveredKeyManagementServiceMachineIpAddress;
        std::string DiscoveredKeyManagementServiceMachineName;
        int DiscoveredKeyManagementServiceMachinePort;
        int IsKeyManagementServiceMachine;
        int KeyManagementServiceCurrentCount;
        bool KeyManagementServiceDnsPublishing;
        int KeyManagementServiceFailedRequests;
        bool KeyManagementServiceHostCaching;
        int KeyManagementServiceLicensedRequests;
        int KeyManagementServiceListeningPort;
        std::string KeyManagementServiceLookupDomain;
        bool KeyManagementServiceLowPriority;
        std::string KeyManagementServiceMachine;
        int KeyManagementServiceNonGenuineGraceRequests;
        int KeyManagementServiceNotificationRequests;
        int KeyManagementServiceOOBGraceRequests;
        int KeyManagementServiceOOTGraceRequests;
        int KeyManagementServicePort;
        std::string KeyManagementServiceProductKeyID;
        int KeyManagementServiceTotalRequests;
        int KeyManagementServiceUnlicensedRequests;
        int OA2xBiosMarkerMinorVersion;
        int OA2xBiosMarkerStatus;
        std::string OA3xOriginalProductKey;
        std::string OA3xOriginalProductKeyDescription;
        std::string OA3xOriginalProductKeyPkPn;
        int PolicyCacheRefreshRequired;
        int RemainingWindowsReArmCount;
        int RequiredClientCount;
        std::string TokenActivationAdditionalInfo;
        int TokenActivationCertificateThumbprint;
        int TokenActivationGrantNumber;
        std::string TokenActivationILID;
        int TokenActivationILVID;
        std::string Version;
        int VLActivationInterval;
        int VLRenewalInterval;
    };

    struct Win32_LogicalDisk {
        Win32_LogicalDisk() :
                Access(), Availability(), BlockSize(), Caption(), Compressed(), ConfigManagerErrorCode(),
                ConfigManagerUserConfig(), CreationClassName(), Description(), DeviceID(), DriveType(),
                ErrorCleared(), ErrorDescription(), ErrorMethodology(), FileSystem(), FreeSpace(), InstallDate(),
                LastErrorCode(), MaximumComponentLength(), MediaType(), Name(), NumberOfBlocks(), PNPDeviceID(),
                PowerManagementCapabilities(), PowerManagementSupported(), ProviderName(), Purpose(), QuotasDisabled(),
                QuotasIncomplete(), QuotasRebuilding(), Size(), Status(), StatusInfo(), SupportsDiskQuotas(),
                SupportsFileBasedCompression(), SystemCreationClassName(), SystemName(), VolumeDirty(),
                VolumeName(), VolumeSerialNumber() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "Access", (*this).Access);
            result.extract(index, "Availability", (*this).Availability);
            result.extract(index, "BlockSize", (*this).BlockSize);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "Compressed", (*this).Compressed);
            result.extract(index, "ConfigManagerErrorCode", (*this).ConfigManagerErrorCode);
            result.extract(index, "ConfigManagerUserConfig", (*this).ConfigManagerUserConfig);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DeviceID", (*this).DeviceID);
            result.extract(index, "DriveType", (*this).DriveType);
            result.extract(index, "ErrorCleared", (*this).ErrorCleared);
            result.extract(index, "ErrorDescription", (*this).ErrorDescription);
            result.extract(index, "ErrorMethodology", (*this).ErrorMethodology);
            result.extract(index, "FileSystem", (*this).FileSystem);
            result.extract(index, "FreeSpace", (*this).FreeSpace);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "LastErrorCode", (*this).LastErrorCode);
            result.extract(index, "MaximumComponentLength", (*this).MaximumComponentLength);
            result.extract(index, "MediaType", (*this).MediaType);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "NumberOfBlocks", (*this).NumberOfBlocks);
            result.extract(index, "PNPDeviceID", (*this).PNPDeviceID);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "ProviderName", (*this).ProviderName);
            result.extract(index, "Purpose", (*this).Purpose);
            result.extract(index, "QuotasDisabled", (*this).QuotasDisabled);
            result.extract(index, "QuotasIncomplete", (*this).QuotasIncomplete);
            result.extract(index, "QuotasRebuilding", (*this).QuotasRebuilding);
            result.extract(index, "Size", (*this).Size);
            result.extract(index, "StatusInfo", (*this).StatusInfo);
            result.extract(index, "SupportsDiskQuotas", (*this).SupportsDiskQuotas);
            result.extract(index, "SupportsFileBasedCompression", (*this).SupportsFileBasedCompression);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "VolumeDirty", (*this).VolumeDirty);
            result.extract(index, "VolumeName", (*this).VolumeName);
            result.extract(index, "VolumeSerialNumber", (*this).VolumeSerialNumber);
        }

        static std::string getWmiClassName() {
            return "Win32_LogicalDisk";
        }

        int Access;
        int Availability;
        std::string BlockSize;
        std::string Caption;
        bool Compressed;
        int ConfigManagerErrorCode;
        bool ConfigManagerUserConfig;
        std::string CreationClassName;
        std::string Description;
        std::string DeviceID;
        int DriveType;
        bool ErrorCleared;
        std::string ErrorDescription;
        std::string ErrorMethodology;
        std::string FileSystem;
        std::string FreeSpace;
        std::string InstallDate;
        int LastErrorCode;
        int MaximumComponentLength;
        int MediaType;
        std::string Name;
        std::string NumberOfBlocks;
        std::string PNPDeviceID;
        std::string PowerManagementCapabilities;
        std::string PowerManagementSupported;
        std::string ProviderName;
        std::string Purpose;
        bool QuotasDisabled;
        bool QuotasIncomplete;
        bool QuotasRebuilding;
        std::string Size;
        std::string Status;
        std::string StatusInfo;
        bool SupportsDiskQuotas;
        bool SupportsFileBasedCompression;
        std::string SystemCreationClassName;
        std::string SystemName;
        bool VolumeDirty;
        std::string VolumeName;
        std::string VolumeSerialNumber;
    }; //end Win32_LogicalDisk

    struct Win32_OperatingSystem {
        Win32_OperatingSystem() :
                BootDevice(),
                BuildNumber(),
                BuildType(),
                Caption(),
                CodeSet(),
                CountryCode(),
                CreationClassName(),
                CSCreationClassName(),
                CSName(),
                CurrentTimeZone(),
                DataExecutionPrevention_32BitApplications(),
                DataExecutionPrevention_Available(),
                DataExecutionPrevention_Drivers(),
                DataExecutionPrevention_SupportPolicy(),
                Debug(),
                Description(),
                Distributed(),
                EncryptionLevel(),
                ForegroundApplicationBoost(),
                FreePhysicalMemory(),
                FreeSpaceInPagingFiles(),
                FreeVirtualMemory(),
                InstallDate(),
                LastBootUpTime(),
                LocalDateTime(),
                Locale(),
                Manufacturer(),
                MaxNumberOfProcesses(),
                MaxProcessMemorySize(),
                MUILanguages(),
                Name(),
                NumberOfProcesses(),
                NumberOfUsers(),
                OperatingSystemSKU(),
                Organization(),
                OSArchitecture(),
                OSLanguage(),
                OSProductSuite(),
                OSType(),
                PortableOperatingSystem(),
                Primary(),
                ProductType(),
                RegisteredUser(),
                SerialNumber(),
                ServicePackMajorVersion(),
                ServicePackMinorVersion(),
                SizeStoredInPagingFiles(),
                Status(),
                SuiteMask(),
                SystemDevice(),
                SystemDirectory(),
                SystemDrive(),
                TotalVirtualMemorySize(),
                TotalVisibleMemorySize(),
                Version(),
                WindowsDirectory() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "BootDevice", (*this).BootDevice);
            result.extract(index, "BuildNumber", (*this).BuildNumber);
            result.extract(index, "BuildType", (*this).BuildType);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "CodeSet", (*this).CodeSet);
            result.extract(index, "CountryCode", (*this).CountryCode);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "CSCreationClassName", (*this).CSCreationClassName);
            result.extract(index, "CSName", (*this).CSName);
            result.extract(index, "CurrentTimeZone", (*this).CurrentTimeZone);
            result.extract(index, "DataExecutionPrevention_32BitApplications",
                           (*this).DataExecutionPrevention_32BitApplications);
            result.extract(index, "DataExecutionPrevention_Available", (*this).DataExecutionPrevention_Available);
            result.extract(index, "DataExecutionPrevention_Drivers", (*this).DataExecutionPrevention_Drivers);
            result.extract(index, "DataExecutionPrevention_SupportPolicy",
                           (*this).DataExecutionPrevention_SupportPolicy);
            result.extract(index, "Debug", (*this).Debug);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "Distributed", (*this).Distributed);
            result.extract(index, "EncryptionLevel", (*this).EncryptionLevel);
            result.extract(index, "ForegroundApplicationBoost", (*this).ForegroundApplicationBoost);
            result.extract(index, "FreePhysicalMemory", (*this).FreePhysicalMemory);
            result.extract(index, "FreeSpaceInPagingFiles", (*this).FreeSpaceInPagingFiles);
            result.extract(index, "FreeVirtualMemory", (*this).FreeVirtualMemory);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "LastBootUpTime", (*this).LastBootUpTime);
            result.extract(index, "LocalDateTime", (*this).LocalDateTime);
            result.extract(index, "Locale", (*this).Locale);
            result.extract(index, "Manufacturer", (*this).Manufacturer);
            result.extract(index, "MaxNumberOfProcesses", (*this).MaxNumberOfProcesses);
            result.extract(index, "MaxProcessMemorySize", (*this).MaxProcessMemorySize);
            result.extract(index, "MUILanguages", (*this).MUILanguages);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "NumberOfProcesses", (*this).NumberOfProcesses);
            result.extract(index, "NumberOfUsers", (*this).NumberOfUsers);
            result.extract(index, "OperatingSystemSKU", (*this).OperatingSystemSKU);
            result.extract(index, "Organization", (*this).Organization);
            result.extract(index, "OSArchitecture", (*this).OSArchitecture);
            result.extract(index, "OSLanguage", (*this).OSLanguage);
            result.extract(index, "OSProductSuite", (*this).OSProductSuite);
            result.extract(index, "OSType", (*this).OSType);
            result.extract(index, "PortableOperatingSystem", (*this).PortableOperatingSystem);
            result.extract(index, "Primary", (*this).Primary);
            result.extract(index, "ProductType", (*this).ProductType);
            result.extract(index, "RegisteredUser", (*this).RegisteredUser);
            result.extract(index, "SerialNumber", (*this).SerialNumber);
            result.extract(index, "ServicePackMajorVersion", (*this).ServicePackMajorVersion);
            result.extract(index, "ServicePackMinorVersion", (*this).ServicePackMinorVersion);
            result.extract(index, "SizeStoredInPagingFiles", (*this).SizeStoredInPagingFiles);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "SuiteMask", (*this).SuiteMask);
            result.extract(index, "SystemDevice", (*this).SystemDevice);
            result.extract(index, "SystemDirectory", (*this).SystemDirectory);
            result.extract(index, "SystemDrive", (*this).SystemDrive);
            result.extract(index, "TotalVirtualMemorySize", (*this).TotalVirtualMemorySize);
            result.extract(index, "TotalVisibleMemorySize", (*this).TotalVisibleMemorySize);
            result.extract(index, "Version", (*this).Version);
            result.extract(index, "WindowsDirectory", (*this).WindowsDirectory);

        }

        static std::string getWmiClassName() {
            return "Win32_OperatingSystem";
        }

        std::string BootDevice;
        std::string BuildNumber;
        std::string BuildType;
        std::string Caption;
        std::string CodeSet;
        std::string CountryCode;
        std::string CreationClassName;
        std::string CSCreationClassName;
        std::string CSName;
        int CurrentTimeZone;
        bool DataExecutionPrevention_32BitApplications;
        bool DataExecutionPrevention_Available;
        bool DataExecutionPrevention_Drivers;
        int DataExecutionPrevention_SupportPolicy;
        bool Debug;
        std::string Description;
        bool Distributed;
        int EncryptionLevel;
        int ForegroundApplicationBoost;
        uint64_t FreePhysicalMemory;
        uint64_t FreeSpaceInPagingFiles;
        uint64_t FreeVirtualMemory;
        /////////////////////////////////////////////////////
        std::string InstallDate;
        std::string LastBootUpTime;///THIS MAYBE INCORRECT
        std::string LocalDateTime;
        /////////////////////////////////////////////////////////
        std::string Locale;
        std::string Manufacturer;
        uint64_t MaxNumberOfProcesses;
        uint64_t MaxProcessMemorySize;
        std::string MUILanguages;
        std::string Name;
        int NumberOfProcesses;
        int NumberOfUsers;
        int OperatingSystemSKU;
        std::string Organization;
        std::string OSArchitecture;
        int OSLanguage;
        int OSProductSuite;
        int OSType;
        bool PortableOperatingSystem;
        bool Primary;
        int ProductType;
        std::string RegisteredUser;
        std::string SerialNumber;
        int ServicePackMajorVersion;
        int ServicePackMinorVersion;
        uint64_t SizeStoredInPagingFiles;
        std::string Status;
        int SuiteMask;
        std::string SystemDevice;
        std::string SystemDirectory;
        std::string SystemDrive;
        uint64_t TotalVirtualMemorySize;
        uint64_t TotalVisibleMemorySize;
        std::string Version;
        std::string WindowsDirectory;
    };

    struct Win32_VideoController {
        Win32_VideoController() :
                AcceleratorCapabilities(), AdapterCompatibility(), AdapterDACType(), AdapterRAM(), Availability(),
                CapabilityDescriptions(), Caption(), ColorTableEntries(), ConfigManagerErrorCode(),
                ConfigManagerUserConfig(),
                CreationClassName(), CurrentBitsPerPixel(), CurrentHorizontalResolution(), CurrentNumberOfColors(),
                CurrentNumberOfColumns(), CurrentNumberOfRows(), CurrentRefreshRate(), CurrentScanMode(),
                CurrentVerticalResolution(),
                Description(), DeviceID(), DeviceSpecificPens(), DitherType(), DriverDate(), DriverVersion(),
                ErrorCleared(),
                ErrorDescription(), ICMIntent(), ICMMethod(), InfFilename(), InfSection(), InstallDate(),
                InstalledDisplayDrivers(),
                LastErrorCode(), MaxMemorySupported(), MaxNumberControlled(), MaxRefreshRate(), MinRefreshRate(),
                Monochrome(), Name(),
                NumberOfColorPlanes(), NumberOfVideoPages(), PNPDeviceID(), PowerManagementCapabilities(),
                PowerManagementSupported(),
                ProtocolSupported(), ReservedSystemPaletteEntries(), SpecificationVersion(), Status(), StatusInfo(),
                SystemCreationClassName(), SystemName(), SystemPaletteEntries(), TimeOfLastReset(), VideoArchitecture(),
                VideoMemoryType(), VideoMode(), VideoModeDescription(), VideoProcessor() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "AcceleratorCapabilities", (*this).AcceleratorCapabilities);
            result.extract(index, "AdapterCompatibility", (*this).AdapterCompatibility);
            result.extract(index, "AdapterDACType", (*this).AdapterDACType);
            result.extract(index, "AdapterRAM", (*this).AdapterRAM);
            result.extract(index, "Availability", (*this).Availability);
            result.extract(index, "CapabilityDescriptions", (*this).CapabilityDescriptions);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "ColorTableEntries", (*this).ColorTableEntries);
            result.extract(index, "ConfigManagerErrorCode", (*this).ConfigManagerErrorCode);
            result.extract(index, "ConfigManagerUserConfig", (*this).ConfigManagerUserConfig);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "CurrentBitsPerPixel", (*this).CurrentBitsPerPixel);
            result.extract(index, "CurrentHorizontalResolution", (*this).CurrentHorizontalResolution);
            result.extract(index, "CurrentNumberOfColors", (*this).CurrentNumberOfColors);
            result.extract(index, "CurrentNumberOfColumns", (*this).CurrentNumberOfColumns);
            result.extract(index, "CurrentNumberOfRows", (*this).CurrentNumberOfRows);
            result.extract(index, "CurrentRefreshRate", (*this).CurrentRefreshRate);
            result.extract(index, "CurrentScanMode", (*this).CurrentScanMode);
            result.extract(index, "CurrentVerticalResolution", (*this).CurrentVerticalResolution);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "DeviceID", (*this).DeviceID);
            result.extract(index, "DeviceSpecificPens", (*this).DeviceSpecificPens);
            result.extract(index, "DitherType", (*this).DitherType);
            result.extract(index, "DriverDate", (*this).DriverDate);
            result.extract(index, "DriverVersion", (*this).DriverVersion);
            result.extract(index, "ErrorCleared", (*this).ErrorCleared);
            result.extract(index, "ErrorDescription", (*this).ErrorDescription);
            result.extract(index, "ICMIntent", (*this).ICMIntent);
            result.extract(index, "ICMMethod", (*this).ICMMethod);
            result.extract(index, "InfFilename", (*this).InfFilename);
            result.extract(index, "InfSection", (*this).InfSection);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "InstalledDisplayDrivers", (*this).InstalledDisplayDrivers);
            result.extract(index, "LastErrorCode", (*this).LastErrorCode);
            result.extract(index, "MaxMemorySupported", (*this).MaxMemorySupported);
            result.extract(index, "MaxNumberControlled", (*this).MaxNumberControlled);
            result.extract(index, "MaxRefreshRate", (*this).MaxRefreshRate);
            result.extract(index, "MinRefreshRate", (*this).MinRefreshRate);
            result.extract(index, "Monochrome", (*this).Monochrome);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "NumberOfColorPlanes", (*this).NumberOfColorPlanes);
            result.extract(index, "NumberOfVideoPages", (*this).NumberOfVideoPages);
            result.extract(index, "PNPDeviceID", (*this).PNPDeviceID);
            result.extract(index, "PowerManagementCapabilities", (*this).PowerManagementCapabilities);
            result.extract(index, "PowerManagementSupported", (*this).PowerManagementSupported);
            result.extract(index, "ProtocolSupported", (*this).ProtocolSupported);
            result.extract(index, "ReservedSystemPaletteEntries", (*this).ReservedSystemPaletteEntries);
            result.extract(index, "SpecificationVersion", (*this).SpecificationVersion);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "StatusInfo", (*this).StatusInfo);
            result.extract(index, "SystemCreationClassName", (*this).SystemCreationClassName);
            result.extract(index, "SystemName", (*this).SystemName);
            result.extract(index, "SystemPaletteEntries", (*this).SystemPaletteEntries);
            result.extract(index, "TimeOfLastReset", (*this).TimeOfLastReset);
            result.extract(index, "VideoArchitecture", (*this).VideoArchitecture);
            result.extract(index, "VideoMemoryType", (*this).VideoMemoryType);
            result.extract(index, "VideoMode", (*this).VideoMode);
            result.extract(index, "VideoModeDescription", (*this).VideoModeDescription);
            result.extract(index, "VideoProcessor", (*this).VideoProcessor);
        }

        static std::string getWmiClassName() {
            return "Win32_VideoController";
        }

        std::string AcceleratorCapabilities;
        std::string AdapterCompatibility;
        std::string AdapterDACType;
        std::uint32_t AdapterRAM;
        std::uint16_t Availability;
        std::string CapabilityDescriptions;
        std::string Caption;
        std::uint32_t ColorTableEntries;
        std::uint32_t ConfigManagerErrorCode;
        bool ConfigManagerUserConfig;
        std::string CreationClassName;
        std::uint32_t CurrentBitsPerPixel;
        std::uint32_t CurrentHorizontalResolution;
        std::uint64_t CurrentNumberOfColors;
        std::uint32_t CurrentNumberOfColumns;
        std::uint32_t CurrentNumberOfRows;
        std::uint32_t CurrentRefreshRate;
        std::uint16_t CurrentScanMode;
        std::uint32_t CurrentVerticalResolution;
        std::string Description;
        std::string DeviceID;
        std::uint32_t DeviceSpecificPens;
        std::uint32_t DitherType;
        std::string DriverDate;
        std::string DriverVersion;
        bool ErrorCleared;
        std::string ErrorDescription;
        std::uint32_t ICMIntent;
        std::uint32_t ICMMethod;
        std::string InfFilename;
        std::string InfSection;
        std::string InstallDate;
        std::string InstalledDisplayDrivers;
        std::uint32_t LastErrorCode;
        std::uint32_t MaxMemorySupported;
        std::uint32_t MaxNumberControlled;
        std::uint32_t MaxRefreshRate;
        std::uint32_t MinRefreshRate;
        bool Monochrome;
        std::string Name;
        std::uint16_t NumberOfColorPlanes;
        std::uint32_t NumberOfVideoPages;
        std::string PNPDeviceID;
        std::string PowerManagementCapabilities;
        bool PowerManagementSupported;
        std::uint16_t ProtocolSupported;
        std::uint32_t ReservedSystemPaletteEntries;
        std::uint32_t SpecificationVersion;
        std::string Status;
        std::uint16_t StatusInfo;
        std::string SystemCreationClassName;
        std::string SystemName;
        std::uint32_t SystemPaletteEntries;
        std::string TimeOfLastReset;
        std::uint16_t VideoArchitecture;
        std::uint16_t VideoMemoryType;
        std::uint16_t VideoMode;
        std::string VideoModeDescription;
        std::string VideoProcessor;
    };

    struct Win32_BaseBoard {
        Win32_BaseBoard() :
                Caption(), ConfigOptions(), CreationClassName(), Depth(), Description(), Height(), HostingBoard(),
                HotSwappable(), InstallDate(),
                Manufacturer(), Model(), Name(), OtherIdentifyingInfo(), PartNumber(), PoweredOn(), Product(),
                Removable(), Replaceable(), RequirementsDescription(),
                RequiresDaughterBoard(), SerialNumber(), SKU(), SlotLayout(), SpecialRequirements(), Status(), Tag(),
                Version(), Weight(), Width() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            //vscode was a great help with search(.+?),
            //and replace result.extract(index, "$1", (*this).$1);
            result.extract(index, "Caption", (*this).Caption);
            result.extract(index, "ConfigOptions", (*this).ConfigOptions);
            result.extract(index, "CreationClassName", (*this).CreationClassName);
            result.extract(index, "Depth", (*this).Depth);
            result.extract(index, "Description", (*this).Description);
            result.extract(index, "Height", (*this).Height);
            result.extract(index, "HostingBoard", (*this).HostingBoard);
            result.extract(index, "HotSwappable", (*this).HotSwappable);
            result.extract(index, "InstallDate", (*this).InstallDate);
            result.extract(index, "Manufacturer", (*this).Manufacturer);
            result.extract(index, "Model", (*this).Model);
            result.extract(index, "Name", (*this).Name);
            result.extract(index, "OtherIdentifyingInfo", (*this).OtherIdentifyingInfo);
            result.extract(index, "PoweredOn", (*this).PoweredOn);
            result.extract(index, "Product", (*this).Product);
            result.extract(index, "Removable", (*this).Removable);
            result.extract(index, "Replaceable", (*this).Replaceable);
            result.extract(index, "RequirementsDescription", (*this).RequirementsDescription);
            result.extract(index, "RequiresDaughterBoard", (*this).RequiresDaughterBoard);
            result.extract(index, "SerialNumber", (*this).SerialNumber);
            result.extract(index, "SKU", (*this).SKU);
            result.extract(index, "SlotLayout", (*this).SlotLayout);
            result.extract(index, "SpecialRequirements", (*this).SpecialRequirements);
            result.extract(index, "Status", (*this).Status);
            result.extract(index, "Tag", (*this).Tag);
            result.extract(index, "Version", (*this).Version);
            result.extract(index, "Weight", (*this).Weight);
            result.extract(index, "Width", (*this).Width);
        }

        static std::string getWmiClassName() {
            return "Win32_BaseBoard";
        }

        std::string Caption;
        std::string ConfigOptions;
        std::string CreationClassName;
        int Depth;
        std::string Description;
        int Height;
        bool HostingBoard;
        bool HotSwappable;
        std::string InstallDate;
        std::string Manufacturer;
        std::string Model;
        std::string Name;
        std::string OtherIdentifyingInfo;
        std::string PartNumber;
        bool PoweredOn;
        std::string Product;
        bool Removable;
        bool Replaceable;
        std::string RequirementsDescription;
        std::string RequiresDaughterBoard;
        std::string SerialNumber;
        std::string SKU;
        std::string SlotLayout;
        std::string SpecialRequirements;
        std::string Status;
        std::string Tag;
        std::string Version;
        int Weight;
        int Width;
    }; //end class Win32_BaseBoard

} //end namespace wmi

#endif //WMICLASSES_HPP