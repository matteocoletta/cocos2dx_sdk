#include "CommandExecutor.h"
#include "Adjust/Adjust2dx.h"
#include "Adjust/AdjustConfig2dx.h"
#include "cocos2d.h"
#include <stdlib.h>

USING_NS_CC;

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
    else if (methodName == "teardown")   Teardown(params);
    else if (methodName == "openDeeplink")   OpenDeeplink(params);
    else if (methodName == "sendReferrer")   SendReferrer(params);
    else if (methodName == "testBegin")   TestBegin(params);
    else if (methodName == "testEnd")   TestEnd(params);
    else {CCLOG("\n[*cocos*] >>>> Undefined JSON command!!"); }
}

void CommandExecutor::Factory(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] factory()");

    if(params.HasMember("basePath")) {
        auto& valBasePath = params["basePath"];
        basePath = valBasePath[0].GetString();
    }

    if(params.HasMember("timerInterval")) {
        auto& val = params["timerInterval"];
        auto timerInterval = val[0].GetDouble();
        Adjust2dx::setTimerInterval(timerInterval);
    }

    if(params.HasMember("timerStart")) {
        auto& val = params["timerStart"];
        auto timerStart = val[0].GetDouble();
        Adjust2dx::setTimerStart(timerStart);
    }

    if(params.HasMember("sessionInterval")) {
        auto& val = params["sessionInterval"];
        auto sessionInterval = val[0].GetDouble();
        Adjust2dx::setSessionInterval(sessionInterval);
    }

    if(params.HasMember("subsessionInterval")) {
        auto& val = params["subsessionInterval"];
        auto subsessionInterval = val[0].GetDouble();
        Adjust2dx::setSubsessionInterval(subsessionInterval);
    }
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

void CommandExecutor::Config(rapidjson::Document& params) {
    AdjustConfig2dx* adjustConfig;
    auto environment = GetFirstParameterValue(params, "environment").GetString();
    auto appToken = GetFirstParameterValue(params, "appToken").GetString();
    adjustConfig = new AdjustConfig2dx(appToken, environment);
    auto logLevel = GetFirstParameterValue(params, "logLevel").GetString();
    adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);

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

    if (params.HasMember("attributionCallbackSendAll")) {
        adjustConfig->setAttributionCallback(OnAttributionChanged);
    }

    if (params.HasMember("sessionCallbackSendSuccess")) {
        adjustConfig->setSessionTrackingSucceededCallbackMethod(OnFinishedSessionTrackingSucceeded);
    }

    if (params.HasMember("sessionCallbackSendFailure")) {
        adjustConfig->setSessionTrackingFailedCallbackMethod(OnFinishedSessionTrackingFailed);
    }

    if (params.HasMember("eventCallbackSendSuccess")) {
        adjustConfig->setEventTrackingSucceededCallbackMethod(OnFinishedEventTrackingSucceeded);
    }

    if (params.HasMember("eventCallbackSendFailure")) {
        adjustConfig->setEventTrackingFailedCallbackMethod(OnFinishedEventTrackingFailed);
    }

    return adjustConfig;
}

void CommandExecutor::Start(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] start()");

    AdjustConfig2dx *adjustConfig = Config(params);

    adjustConfig->setBasePath(basePath);
    Adjust2dx::start(*adjustConfig);
}

AdjustEvent2dx* CommandExecutor::Event(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] event()");

    AdjustEvent2dx *adjustEvent;
    auto eventToken = GetFirstParameterValue(params, "eventToken").GetString();
    adjustEvent = new AdjustEvent2dx(eventToken);

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

    return adjustEvent;
}

void CommandExecutor::TrackEvent(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] trackevent()");

    AdjustEvent2dx* adjustEvent = Event(params);
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

    if (params.HasMember("key")) {
        auto& values = params["key"];
        for (rapidjson::SizeType i = 0; i < values.Size(); i++){
            auto key = values[i].GetString();
            CCLOG("{key=%s}", key);
            Adjust2dx::removeSessionPartnerParameter(key);
        }
    }
}

void CommandExecutor::RemoveSessionPartnerParameter(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] RemoveSessionPartnerParameter()");

    if (params.HasMember("key")) {
        auto& values = params["key"];
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
}

void CommandExecutor::TestEnd(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] testEnd()");
    Adjust2dx::teardown(true);
}

static void OnAttributionChanged(AdjustAttribution& attribution)
{
    AppDelegate::addInfoToSend("trackerToken", attribution.getTrackerToken());
    AppDelegate::addInfoToSend("trackerName", attribution.getTrackerName());
    AppDelegate::addInfoToSend("network", attribution.getNetwork());
    AppDelegate::addInfoToSend("campaign", attribution.getCampaign());
    AppDelegate::addInfoToSend("adgroup", attribution.getAdGroup());
    AppDelegate::addInfoToSend("creative", attribution.getCreative());
    AppDelegate::addInfoToSend("clickLabel", attribution.getClickLabel());
    AppDelegate::addInfoToSend("adid", attribution.getAdid());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedSessionTrackingFailed(AdjustSessionFailure& sessionFail)
{
    AppDelegate::addInfoToSend("message", sessionFail.getMessage());
    AppDelegate::addInfoToSend("timestamp", sessionFail.getTimeStamp());
    AppDelegate::addInfoToSend("adid", sessionFail.getAdid());
    AppDelegate::addInfoToSend("willRetry", sessionFail.getWillRetry().toString());
    AppDelegate::addInfoToSend("jsonResponse", sessionFail.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedSessionTrackingSucceeded(AdjustSessionSuccess& sessionSuccess)
{
    AppDelegate::addInfoToSend("message", sessionSuccess.getMessage());
    AppDelegate::addInfoToSend("timestamp", sessionSuccess.getTimeStamp());
    AppDelegate::addInfoToSend("adid", sessionSuccess.getAdid());
    AppDelegate::addInfoToSend("jsonResponse", sessionSuccess.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedEventTrackingFailed(AdjustEventFailure& eventFail)
{
    AppDelegate::addInfoToSend("message", eventFail.getMessage());
    AppDelegate::addInfoToSend("timestamp", eventFail.getTimeStamp());
    AppDelegate::addInfoToSend("adid", eventFail.getAdid());
    AppDelegate::addInfoToSend("eventToken", eventFail.getEventToken());
    AppDelegate::addInfoToSend("willRetry", eventFail.getWillRetry().toString());
    AppDelegate::addInfoToSend("jsonResponse", eventFail.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedEventTrackingSucceeded(AdjustEventSuccess eventSuccess)
{
    AppDelegate::addInfoToSend("message", eventSuccess.getMessage());
    AppDelegate::addInfoToSend("timestamp", eventSuccess.getTimeStamp());
    AppDelegate::addInfoToSend("adid", eventSuccess.getAdid());
    AppDelegate::addInfoToSend("eventToken", eventSuccess.getEventToken());
    AppDelegate::addInfoToSend("jsonResponse", eventSuccess.getJsonResponse());

    AppDelegate::sendInfoToServer();
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
