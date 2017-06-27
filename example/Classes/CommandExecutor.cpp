#include "CommandExecutor.h"
#include "Adjust/Adjust2dx.h"
#include "Adjust/AdjustConfig2dx.h"
#include "Adjust/AdjustEvent2dx.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include <stdlib.h>
#include <string>
#include <sstream>

USING_NS_CC;
static void OnAttributionChanged(AdjustAttribution2dx attribution)
{
    CCLOG("\n[*cocos*] OnAttributionChanged()");
    AppDelegate::addInfoToSend("trackerToken", attribution.getTrackerToken());
    AppDelegate::addInfoToSend("trackerName", attribution.getTrackerName());
    AppDelegate::addInfoToSend("network", attribution.getNetwork());
    AppDelegate::addInfoToSend("campaign", attribution.getCampaign());
    AppDelegate::addInfoToSend("adgroup", attribution.getAdgroup());
    AppDelegate::addInfoToSend("creative", attribution.getCreative());
    AppDelegate::addInfoToSend("clickLabel", attribution.getClickLabel());
    AppDelegate::addInfoToSend("adid", attribution.getAdid());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedSessionTrackingFailed(AdjustSessionFailure2dx sessionFail)
{
    CCLOG("\n[*cocos*] OnFinishedSessionTrackingFailed()");
    AppDelegate::addInfoToSend("message", sessionFail.getMessage());
    AppDelegate::addInfoToSend("timestamp", sessionFail.getTimestamp());
    AppDelegate::addInfoToSend("adid", sessionFail.getAdid());
    AppDelegate::addInfoToSend("willRetry", sessionFail.getWillRetry());
    AppDelegate::addInfoToSend("jsonResponse", sessionFail.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedSessionTrackingSucceeded(AdjustSessionSuccess2dx sessionSuccess)
{
    CCLOG("\n[*cocos*] OnFinishedSessionTrackingSucceeded()");
    AppDelegate::addInfoToSend("message", sessionSuccess.getMessage());
    AppDelegate::addInfoToSend("timestamp", sessionSuccess.getTimestamp());
    AppDelegate::addInfoToSend("adid", sessionSuccess.getAdid());
    AppDelegate::addInfoToSend("jsonResponse", sessionSuccess.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedEventTrackingFailed(AdjustEventFailure2dx eventFail)
{
    CCLOG("\n[*cocos*] OnFinishedEventTrackingFailed()");
    AppDelegate::addInfoToSend("message", eventFail.getMessage());
    AppDelegate::addInfoToSend("timestamp", eventFail.getTimestamp());
    AppDelegate::addInfoToSend("adid", eventFail.getAdid());
    AppDelegate::addInfoToSend("eventToken", eventFail.getEventToken());
    AppDelegate::addInfoToSend("willRetry", eventFail.getWillRetry());
    AppDelegate::addInfoToSend("jsonResponse", eventFail.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

static void OnFinishedEventTrackingSucceeded(AdjustEventSuccess2dx eventSuccess)
{
    CCLOG("\n[*cocos*] OnFinishedEventTrackingSucceeded()");
    AppDelegate::addInfoToSend("message", eventSuccess.getMessage());
    AppDelegate::addInfoToSend("timestamp", eventSuccess.getTimestamp());
    AppDelegate::addInfoToSend("adid", eventSuccess.getAdid());
    AppDelegate::addInfoToSend("eventToken", eventSuccess.getEventToken());
    AppDelegate::addInfoToSend("jsonResponse", eventSuccess.getJsonResponse());

    AppDelegate::sendInfoToServer();
}

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
    else if (methodName == "pause")   Pause(params);
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
        basePath = GetValue(0, params, "basePath");
    }

    if(params.HasMember("timerInterval")) {
        auto timerInterval = ToDouble(GetValue(0, params, "timerInterval"));
        Adjust2dx::setTimerInterval(timerInterval);
    }

    if(params.HasMember("timerStart")) {
        auto timerStart = ToDouble(GetValue(0, params, "timerStart"));
        Adjust2dx::setTimerStart(timerStart);
    }

    if(params.HasMember("sessionInterval")) {
        auto sessionInterval = ToDouble(GetValue(0, params, "sessionInterval"));
        Adjust2dx::setSessionInterval(sessionInterval);
    }

    if(params.HasMember("subsessionInterval")) {
        auto subsessionInterval = ToDouble(GetValue(0, params, "subsessionInterval"));
        Adjust2dx::setSubsessionInterval(subsessionInterval);
    }
}

void CommandExecutor::Teardown(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] teardown()");

    if(params.HasMember("deleteState")) {
        bool deleteState = ToBool(GetValue(0, params, "deleteState"));
        CCLOG("\n[*cocos*] teardown with deleteState: %s", deleteState ? "true":"false");
        Adjust2dx::teardown(deleteState);
    }
}

AdjustConfig2dx* CommandExecutor::Config(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] Config()");
    AdjustConfig2dx* adjustConfig;
    auto environment = GetValue(0, params, "environment");
    auto appToken = GetValue(0, params, "appToken");
    adjustConfig = new AdjustConfig2dx(appToken, environment);
    auto logLevel = GetValue(0, params, "logLevel");
    adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);

    if (params.HasMember("logLevel")) {
        auto logLevel = GetValue(0, params, "logLevel");
        if (logLevel == "verbose") adjustConfig->setLogLevel(AdjustLogLevel2dxVerbose);
        if (logLevel == "debug") adjustConfig->setLogLevel(AdjustLogLevel2dxDebug);
        if (logLevel == "info") adjustConfig->setLogLevel(AdjustLogLevel2dxInfo);
        if (logLevel == "warn") adjustConfig->setLogLevel(AdjustLogLevel2dxWarn);
        if (logLevel == "error") adjustConfig->setLogLevel(AdjustLogLevel2dxError);
        if (logLevel == "assert") adjustConfig->setLogLevel(AdjustLogLevel2dxAssert);
        if (logLevel == "suppress") adjustConfig->setLogLevel(AdjustLogLevel2dxSuppress);
    }

    if (params.HasMember("defaultTracker")) {
        auto defaultTracker = GetValue(0, params, "defaultTracker");
        adjustConfig->setDefaultTracker(defaultTracker);
    }

    if (params.HasMember("delayStart")) {
        auto delayStartS = GetValue(0, params, "delayStart");
        auto delayStart = atof(delayStartS.c_str());
        adjustConfig->setDelayStart(delayStart);
    }

    if (params.HasMember("eventBufferingEnabled")) {
        auto eventBufferingEnabledS = GetValue(0, params, "eventBufferingEnabled");
        bool eventBufferingEnabled = ToBool(eventBufferingEnabledS);
        adjustConfig->setEventBufferingEnabled(eventBufferingEnabled);
    }

    if (params.HasMember("sendInBackground")) {
        auto sendInBackgroundS = GetValue(0, params, "sendInBackground");
        bool sendInBackground = ToBool(sendInBackgroundS);
        adjustConfig->setSendInBackground(sendInBackground);
    }

    if (params.HasMember("userAgent")) {
        auto userAgent = GetValue(0, params, "userAgent");
        adjustConfig->setUserAgent(userAgent);
    }

    if (params.HasMember("attributionCallbackSendAll")) {
        adjustConfig->setAttributionCallback(OnAttributionChanged);
    }

    if (params.HasMember("sessionCallbackSendSuccess")) {
        adjustConfig->setSessionSuccessCallback(OnFinishedSessionTrackingSucceeded);
    }

    if (params.HasMember("sessionCallbackSendFailure")) {
        adjustConfig->setSessionFailureCallback(OnFinishedSessionTrackingFailed);
    }

    if (params.HasMember("eventCallbackSendSuccess")) {
        adjustConfig->setEventSuccessCallback(OnFinishedEventTrackingSucceeded);
    }

    if (params.HasMember("eventCallbackSendFailure")) {
        adjustConfig->setEventFailureCallback(OnFinishedEventTrackingFailed);
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
    auto eventToken = GetValue(0, params, "eventToken");
    //std::string eventTokenS = eventToken.IsString() ? eventToken : std::string("");
    adjustEvent = new AdjustEvent2dx(eventToken);

    if (params.HasMember("revenue")) {
        auto currency = GetValue(0, params, "revenue");
        auto revenue = GetValue(1, params, "revenue");
        auto fltRevenue = atof(revenue.c_str());
        adjustEvent->setRevenue(fltRevenue, currency);
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

    auto referrer = GetValue(0, params, "referrer");
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

    bool enabled = ToBool(GetValue(0, params, "enabled"));
    Adjust2dx::setEnabled(enabled);
}

void CommandExecutor::SetOfflineMode(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] SetOfflineMode()");

    bool enabled = ToBool(GetValue(0, params, "enabled"));
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

    auto token = GetValue(0, params, "pushToken");
    Adjust2dx::setDeviceToken(token);
}

void CommandExecutor::OpenDeeplink(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] OpenDeeplink()");

    auto deeplink = GetValue(0, params, "deeplink");
    Adjust2dx::appWillOpenUrl(deeplink);
}

void CommandExecutor::SendReferrer(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] sendReferrer()");

    auto referrer = GetValue(0, params, "referrer");
    Adjust2dx::setReferrer(referrer);
}

void CommandExecutor::TestBegin(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] testBegin()");

    if(params.HasMember("basePath")) {
        basePath = GetValue(0, params, "basePath");
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

bool CommandExecutor::ToBool(std::string const& s) {
    return s != "false";
}

double CommandExecutor::ToDouble(std::string const& s) {
    double valor;

    std::stringstream stream(s);
    stream >> valor;
    if (stream.fail()) {
        std::runtime_error e(s);
        throw e;
    }
    return valor;
}

std::string CommandExecutor::GetValue(int idx, rapidjson::Document& params, std::string const& key) {
    if(!params.HasMember(key.c_str())) {
        return std::string("");
    }

    auto& val = params[key.c_str()];
    assert(val.Size() >= idx + 1);
    if(val[idx].IsString()) {
        return val[idx].GetString();
    } else {
        return std::string("");
    }
}
