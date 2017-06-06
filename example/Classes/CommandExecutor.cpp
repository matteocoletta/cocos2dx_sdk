#include "CommandExecutor.h"
#include "Adjust/Adjust2dx.h"
#include "Adjust/AdjustConfig2dx.h"
#include "cocos2d.h"
#include <stdlib.h>

USING_NS_CC;

const std::string CommandExecutor::DefaultConfigName = "defaultConfig";
const std::string CommandExecutor::DefaultEventName = "defaultEvent";

void CommandExecutor::ExecuteCommand(std::string className, std::string methodName, std::string jsonParams) {
    CCLOG("\n[*cocos*] executeCommand()");
    rapidjson::Document params;
    params.Parse(jsonParams.c_str());

    if (methodName == "teardown")   Teardown(params);
    else if (methodName == "factory")   Factory(params);
    else if (methodName == "config")   Config(params);
    else if (methodName == "start")   Start(params);
    else if (methodName == "event")   Event(params);
    else if (methodName == "trackEvent")   TrackEvent(params);
    else if (methodName == "setReferrer")   SetReferrer(params);
    else if (methodName == "pause")   Config(params);
    else if (methodName == "resume")   Resume(params);
    else if (methodName == "setEnabled")   SetEnabled(params);
    else if (methodName == "setOfflineMode")   SetOfflineMode(params);
    else if (methodName == "sendFirstPackages")   SendFirstPackages(params);
    else if (methodName == "addSessionCallbackParameter")   AddSessionCallbackParameter(params);
    else if (methodName == "addSessionPartnerParameter")   AddSessionPartnerParameter(params);
    else if (methodName == "removeSessionCallbackParameter")   RemoveSessionCallbackParameter(params);
    else if (methodName == "removeSessionPartnerParameter")   RemoveSessionPartnerParameter(params);
    else if (methodName == "resetSessionCallbackParameters")   ResetSessionCallbackParameters(params);
    else if (methodName == "resetSessionPartnerParameters")   ResetSessionPartnerParameters(params);
    else if (methodName == "setPushToken")   SetPushToken(params);
    else {CCLOG("\n[*cocos*] >>>> Undefined JSON command!!"); }
}

void CommandExecutor::Teardown(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] teardown()");

    if(params.HasMember("deleteState")) {
        auto& valDeleteState = params["deleteState"];
        bool deleteState = to_bool(valDeleteState[0].GetString());
        CCLOG("\n[*cocos*] teardown with deleteState: %s", deleteState ? "true":"false");
        Adjust2dx::teardown(deleteState);
    }
}

void CommandExecutor::Factory(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] factory()");

    if(params.HasMember("basePath")) {
        auto& valBasePath = params["basePath"];
        basePath = valBasePath[0].GetString();
    }
}

void CommandExecutor::Config(rapidjson::Document& params) {
    std::string configName;
    if (params.HasMember("configName")) {
        configName = params["configName"][0].GetString();
    } else {
        configName = DefaultConfigName;
    }

    AdjustConfig2dx* adjustConfig;
    auto it = savedInstances.find(configName);
    if(it != savedInstances.end()) {
        adjustConfig = static_cast<AdjustConfig2dx*>(it->second); 
    } else {
        auto environment = params["environment"][0].GetString();
        auto appToken = params["appToken"][0].GetString();
        adjustConfig = new AdjustConfig2dx(appToken, environment);
        auto logLevel = params["logLevel"][0].GetString();
        adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);

        savedInstances[configName] = static_cast<void*>(adjustConfig);
    }

    if (params.HasMember("logLevel")) {
        auto logLevel = params["logLevel"][0].GetString();
        if (logLevel == "verbose") adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);
        if (logLevel == "debug") adjustConfig->setLogLevel(AdjustLogLevel2dxDebug);
        if (logLevel == "info") adjustConfig->setLogLevel(AdjustLogLevel2dxInfo);
        if (logLevel == "warn") adjustConfig->setLogLevel(AdjustLogLevel2dxWarn);
        if (logLevel == "error") adjustConfig->setLogLevel(AdjustLogLevel2dxError);
        if (logLevel == "assert") adjustConfig->setLogLevel(AdjustLogLevel2dxAssert);
        if (logLevel == "suppress") adjustConfig->setLogLevel(AdjustLogLevel2dxSuppress);
    }

    if (params.HasMember("defaultTracker")) {
        auto defaultTracker = params["defaultTracker"][0].GetString();
        adjustConfig->setDefaultTracker(defaultTracker);
    }

    if (params.HasMember("delayStart")) {
        auto delayStartS = params["delayStart"][0].GetString();
        auto delayStart = atof(delayStartS);
        adjustConfig->setDelayStart(delayStart);
    }

    if (params.HasMember("eventBufferingEnabled")) {
        auto eventBufferingEnabledS = params["eventBufferingEnabled"][0].GetString();
        bool eventBufferingEnabled = to_bool(eventBufferingEnabledS);
        adjustConfig->setEventBufferingEnabled(eventBufferingEnabled);
    }

    if (params.HasMember("sendInBackground")) {
        auto sendInBackgroundS = params["sendInBackground"][0].GetString();
        bool sendInBackground = to_bool(sendInBackgroundS);
        adjustConfig->setSendInBackground(sendInBackground);
    }

    if (params.HasMember("userAgent")) {
        auto userAgent = params["userAgent"][0].GetString();
        adjustConfig->setUserAgent(userAgent);
    }
}

void CommandExecutor::Start(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] start()");

    Config(params);
    std::string configName;
    if (params.HasMember("configName")) {
        configName = params["configName"][0].GetString();
    } else {
        configName = DefaultConfigName;
    }

    AdjustConfig2dx* adjustConfig;
    auto it = savedInstances.find(configName);
    if(it != savedInstances.end())
    {
        adjustConfig = static_cast<AdjustConfig2dx*>(it->second); 
    } else {
        CCLOG("\n[*cocos*] start() ERROR!! Couldn't find saved instance of adjustConfig");
        return;
    }

    adjustConfig->setBasePath(basePath);
    Adjust2dx::start(*adjustConfig);
}

void CommandExecutor::Event(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] event()");

    std::string eventName;
    if (params.HasMember("eventName")) {
        eventName = params["eventName"][0].GetString();
    } else {
        eventName = DefaultEventName;
    }

    AdjustEvent2dx *adjustEvent;

    std::map<std::string, void*>::iterator it = savedInstances.find(eventName);
    if(it != savedInstances.end())
    {
        adjustEvent = static_cast<AdjustEvent2dx*>(it->second); 
    } else {
        auto eventToken = params["eventToken"][0].GetString();
        adjustEvent = new AdjustEvent2dx(eventToken);
        savedInstances[eventName] = adjustEvent;
    }

    if (params.HasMember("revenue")) {
        auto& revenueParams = params["revenue"];
        auto currency = params["revenue"][0].GetString();
        auto strRevenue = params["revenue"][1].GetString();
        auto revenue = std::atof(strRevenue);
        adjustEvent->setRevenue(revenue, currency);
    }

    if (params.HasMember("callbackParams")) {
        auto& callbackParams = params["callbackParams"];
        for (rapidjson::SizeType i = 0; i < callbackParams.Size(); i = i + 2) {
            auto key = callbackParams[i].GetString();
            auto value = callbackParams[i + 1].GetString();
            adjustEvent->addCallbackParameter(key, value);
        }
    }
    if (params.HasMember("partnerParams")) {
        auto& partnerParams = params["partnerParams"];
        for (rapidjson::SizeType i = 0; i < partnerParams.Size(); i = i + 2) {
            auto key = partnerParams[i].GetString();
            auto value = partnerParams[i + 1].GetString();
            adjustEvent->addPartnerParameter(key, value);
        }
    }
}

void CommandExecutor::TrackEvent(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] trackevent()");

    Event(params);
    std::string eventName;
    if (params.HasMember("eventName")) {
        eventName = params["eventName"][0].GetString();
    } else {
        eventName = DefaultEventName;
    }
    AdjustEvent2dx* adjustEvent = static_cast<AdjustEvent2dx*>(savedInstances[eventName]); 
    Adjust2dx::trackEvent(*adjustEvent);
}

void CommandExecutor::SetReferrer(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] setReferrer()");

    auto referrer = params["referrer"][0].GetString();
    Adjust2dx::setReferrer(referrer);
}

void CommandExecutor::Pause(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] Pause()");

    Adjust2dx::onPause();
}

void CommandExecutor::Resume(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] Resume()");

    Adjust2dx::onResume();
}

void CommandExecutor::SetEnabled(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SetEnabled()");

    bool enabled = to_bool(params["enabled"][0].GetString());
    Adjust2dx::setEnabled(enabled);
}

void CommandExecutor::SetOfflineMode(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SetOfflineMode()");

    bool enabled = to_bool(params["enabled"][0].GetString());
    Adjust2dx::setOfflineMode(enabled);
}

void CommandExecutor::SendFirstPackages(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SendFirstPackages()");

    Adjust2dx::sendFirstPackages();
}

void CommandExecutor::AddSessionCallbackParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] AddSessionCallbackParameter()");

    if (params.HasMember("KeyValue")) {
        auto& values = params["KeyValue"];
        for (SizeType i = 0; i < values.Size(); i = i + 2){
            auto key = values[i];
            auto value = values[i+1];
            CCLOG("{key=%s, value=%s}", key.c_str(), value.c_str());
            Adjust2dx::addSessionCallbackParameter(key, value);
        }
    }
}

void CommandExecutor::AddSessionPartnerParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] AddSessionPartnerParameter()");

    if (params.HasMember("KeyValue")) {
        auto& values = params["KeyValue"];
        for (SizeType i = 0; i < values.Size(); i = i + 2){
            auto key = values[i];
            auto value = values[i+1];
            CCLOG("{key=%s, value=%s}", key.c_str(), value.c_str());
            Adjust2dx::addSessionPartnerParameter(key, value);
        }
    }
}

void CommandExecutor::RemoveSessionCallbackParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] RemoveSessionCallbackParameter()");

    auto key = params["key"][0].GetString();
    Adjust2dx::removeSessionCallbackParameter(key);
}

void CommandExecutor::RemoveSessionPartnerParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] RemoveSessionPartnerParameter()");

    auto key = params["key"][0].GetString();
    Adjust2dx::removeSessionPartnerParameter(key);
}

void CommandExecutor::ResetSessionCallbackParameters(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] ResetSessionCallbackParameters()");

    Adjust2dx::resetSessionCallbackParameters();
}

void CommandExecutor::ResetSessionPartnerParameters(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] ResetSessionPartnerParameters()");

    Adjust2dx::resetSessionPartnerParameters();
}

void CommandExecutor::SetPushToken(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SetPushToken()");

    auto token = params["pushToken"][0].GetString();
    Adjust2dx::setDeviceToken(token);
}

bool CommandExecutor::to_bool(std::string const& s) {
    return s != "false";
}
