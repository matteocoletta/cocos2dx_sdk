#include "CommandExecutor.h"
#include "Adjust/Adjust2dx.h"
#include "cocos2d.h"

USING_NS_CC;

const std::string CommandExecutor::DefaultConfigName = "defaultConfig";
const std::string CommandExecutor::DefaultEventName = "defaultEvent";

void CommandExecutor::ExecuteCommand(std::string className, std::string methodName, std::string jsonParams) {
    CCLOG("\n[*cocos*] executeCommand()");
    rapidjson::Document params;
    params.Parse(jsonParams.c_str());

    if (methodName == "teardown")   Teardown(params);
    //case "factory"                        : factory(params); break;
    //case std::stoi("teardown")                : Teardown(params); break;
    //case "config"                         : config(params); break;
    //case "start"                          : start(params); break;
    //case "event"                          : event(params); break;
    //case "trackEvent"                     : trackEvent(params); break;
    //case "resume"                         : resume(params); break;
    //case "pause"                          : pause(params); break;
    //case "setEnabled"                     : setEnabled(params); break;
    //case "setReferrer"                    : setReferrer(params); break;
    //case "setOfflineMode"                 : setOfflineMode(params); break;
    //case "sendFirstPackages"              : sendFirstPackages(params); break;
    //case "addSessionCallbackParameter"    : this.addSessionCallbackParameter(params); break;
    //case "addSessionPartnerParameter"     : this.addSessionPartnerParameter(params); break;
    //case "removeSessionCallbackParameter" : this.removeSessionCallbackParameter(params); break;
    //case "removeSessionPartnerParameter"  : this.removeSessionPartnerParameter(params); break;
    //case "resetSessionCallbackParameters" : this.resetSessionCallbackParameters(params); break;
    //case "resetSessionPartnerParameters"  : this.resetSessionPartnerParameters(params); break;
    //case "setPushToken"                   : this.setPushToken(params); break;
}

void CommandExecutor::Teardown(rapidjson::Document& params) {
    CCLOG("\n[*cocos*] teardown()");

    if(params.HasMember("deleteState")) {
        auto& valDeleteState = params["deleteState"];
        auto deleteState = valDeleteState[0].GetBool();
        //CCLOG("\n[*cocos*] teardown with deleteState: %b", deleteState);
    }
}
