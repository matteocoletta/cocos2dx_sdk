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
        bool deleteState = ToBool(valDeleteState[0].GetString());
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
        configName = GetFirstParameterValue(params, "configName").GetString();
    } else {
        configName = DefaultConfigName;
    }

    AdjustConfig2dx* adjustConfig;
    auto it = savedInstances.find(configName);
    if(it != savedInstances.end()) {
        adjustConfig = static_cast<AdjustConfig2dx*>(it->second); 
    } else {
        auto environment = GetFirstParameterValue(params, "environment").GetString();
        auto appToken = GetFirstParameterValue(params, "appToken").GetString();
        adjustConfig = new AdjustConfig2dx(appToken, environment);
        auto logLevel = GetFirstParameterValue(params, "logLevel").GetString();
        adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);

        savedInstances[configName] = static_cast<void*>(adjustConfig);
    }

    if (params.HasMember("logLevel")) {
        auto logLevel = GetFirstParameterValue(params, "logLevel").GetString();
        if (logLevel == "verbose") adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);
        if (logLevel == "debug") adjustConfig->setLogLevel(AdjustLogLevel2dxDebug);
        if (logLevel == "info") adjustConfig->setLogLevel(AdjustLogLevel2dxInfo);
        if (logLevel == "warn") adjustConfig->setLogLevel(AdjustLogLevel2dxWarn);
        if (logLevel == "error") adjustConfig->setLogLevel(AdjustLogLevel2dxError);
        if (logLevel == "assert") adjustConfig->setLogLevel(AdjustLogLevel2dxAssert);
        if (logLevel == "suppress") adjustConfig->setLogLevel(AdjustLogLevel2dxSuppress);
    }

    if (params.HasMember("defaultTracker")) {
        auto defaultTracker = GetFirstParameterValue(params, "defaultTracker").GetString();
        adjustConfig->setDefaultTracker(defaultTracker);
    }

    if (params.HasMember("delayStart")) {
        auto delayStartS = GetFirstParameterValue(params, "delayStart").GetString();
        auto delayStart = atof(delayStartS);
        adjustConfig->setDelayStart(delayStart);
    }

    if (params.HasMember("eventBufferingEnabled")) {
        auto eventBufferingEnabledS = GetFirstParameterValue(params, "eventBufferingEnabled").GetString();
        bool eventBufferingEnabled = ToBool(eventBufferingEnabledS);
        adjustConfig->setEventBufferingEnabled(eventBufferingEnabled);
    }

    if (params.HasMember("sendInBackground")) {
        auto sendInBackgroundS = GetFirstParameterValue(params, "sendInBackground").GetString();
        bool sendInBackground = ToBool(sendInBackgroundS);
        adjustConfig->setSendInBackground(sendInBackground);
    }

    if (params.HasMember("userAgent")) {
        auto userAgent = GetFirstParameterValue(params, "userAgent").GetString();
        adjustConfig->setUserAgent(userAgent);
    }
}

void CommandExecutor::Start(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] start()");

    Config(params);
    std::string configName;
    if (params.HasMember("configName")) {
        configName = GetFirstParameterValue(params, "configName").GetString();
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
        eventName = GetFirstParameterValue(params, "eventName").GetString();
    } else {
        eventName = DefaultEventName;
    }

    AdjustEvent2dx *adjustEvent;

    std::map<std::string, void*>::iterator it = savedInstances.find(eventName);
    if(it != savedInstances.end())
    {
        adjustEvent = static_cast<AdjustEvent2dx*>(it->second); 
    } else {
        auto eventToken = GetFirstParameterValue(params, "eventToken").GetString();
        adjustEvent = new AdjustEvent2dx(eventToken);
        savedInstances[eventName] = adjustEvent;
    }

    if (params.HasMember("revenue")) {
        auto& revenueParams = params["revenue"];
        auto currency = GetFirstParameterValue(params, "revenue").GetString();
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
        eventName = GetFirstParameterValue(params, "eventName").GetString();
    } else {
        eventName = DefaultEventName;
    }
    AdjustEvent2dx* adjustEvent = static_cast<AdjustEvent2dx*>(savedInstances[eventName]); 
    Adjust2dx::trackEvent(*adjustEvent);
}

void CommandExecutor::SetReferrer(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] setReferrer()");

    auto referrer = GetFirstParameterValue(params, "referrer").GetString();
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

    bool enabled = ToBool(GetFirstParameterValue(params, "enabled").GetString());
    Adjust2dx::setEnabled(enabled);
}

void CommandExecutor::SetOfflineMode(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SetOfflineMode()");

    bool enabled = ToBool(GetFirstParameterValue(params, "enabled").GetString());
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
        for (rapidjson::SizeType i = 0; i < values.Size(); i = i + 2){
            auto key = values[i].GetString();
            auto value = values[i+1].GetString();
            CCLOG("{key=%s, value=%s}", key, value);
            Adjust2dx::addSessionCallbackParameter(key, value);
        }
    }
}

void CommandExecutor::AddSessionPartnerParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] AddSessionPartnerParameter()");

    if (params.HasMember("KeyValue")) {
        auto& values = params["KeyValue"];
        for (rapidjson::SizeType i = 0; i < values.Size(); i = i + 2){
            auto key = values[i].GetString();
            auto value = values[i+1].GetString();
            CCLOG("{key=%s, value=%s}", key, value);
            Adjust2dx::addSessionPartnerParameter(key, value);
        }
    }
}

void CommandExecutor::RemoveSessionCallbackParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] RemoveSessionCallbackParameter()");

    if (params.HasMember("Key")) {
        auto& values = params["Key"];
        for (rapidjson::SizeType i = 0; i < values.Size(); i++){
            auto key = values[i].GetString();
            CCLOG("{key=%s}", key);
            Adjust2dx::removeSessionPartnerParameter(key);
        }
    }
}

void CommandExecutor::RemoveSessionPartnerParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] RemoveSessionPartnerParameter()");

    if (params.HasMember("Key")) {
        auto& values = params["Key"];
        for (rapidjson::SizeType i = 0; i < values.Size(); i++){
            auto key = values[i].GetString();
            CCLOG("{key=%s}", key);
            Adjust2dx::removeSessionCallbackParameter(key);
        }
    }
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

    auto token = GetFirstParameterValue(params, "pushToken").GetString();
    Adjust2dx::setDeviceToken(token);
}

void CommandExecutor::OpenDeeplink(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] OpenDeeplink()");

    auto deeplink = GetFirstParameterValue(params, "deeplink").GetString();
    Adjust2dx::appWillOpenUrl(deeplink);
}

void CommandExecutor::SendReferrer(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] sendReferrer()");

    auto referrer = GetFirstParameterValue(params, "referrer").GetString();
    Adjust2dx::setReferrer(referrer);
}

void CommandExecutor::TestBegin(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] testBegin()");

    if(params.HasMember("basePath")) {
        auto& valBasePath = params["basePath"];
        basePath = valBasePath[0].GetString();
    }

    Adjust2dx::teardown(true);
    Adjust2dx::setTimerInterval(-1);
    Adjust2dx::setTimerStart(-1);
    Adjust2dx::setSessionInterval(-1);
    Adjust2dx::setSubsessionInterval(-1);

    savedInstances.clear();
}

void CommandExecutor::TestEnd(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] testEnd()");
    Adjust2dx::teardown(true);
}

bool CommandExecutor::ToBool(std::string const& s) {
    return s != "false";
}

rapidjson::Value& CommandExecutor::GetFirstParameterValue(rapidjson::Document& params, std::string const& key) {
    assert(params.HasMember(key.c_str()));
    auto& val = params[key.c_str()];
    assert(val.Size() > 1);
    return val[0];
}
