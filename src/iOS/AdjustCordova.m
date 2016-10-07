//
//  AdjustCordova.m
//  Adjust
//
//  Created by Pedro Filipe on 04/03/14.
//  Copyright (c) 2012-2014 adjust GmbH. All rights reserved.
//

#import <Cordova/CDVPluginResult.h>
#import "AdjustCordova.h"

#define KEY_APP_TOKEN               @ "appToken"
#define KEY_ENVIRONMENT             @ "environment"
#define KEY_LOG_LEVEL               @ "logLevel"
#define KEY_SDK_PREFIX              @ "sdkPrefix"
#define KEY_DEFAULT_TRACKER         @ "defaultTracker"
#define KEY_EVENT_BUFFERING_ENABLED @ "eventBufferingEnabled"
#define KEY_EVENT_TOKEN             @ "eventToken"
#define KEY_REVENUE                 @ "revenue"
#define KEY_CURRENCY                @ "currency"
#define KEY_RECEIPT                 @ "receipt"
#define KEY_TRANSACTION_ID          @ "transactionId"
#define KEY_CALLBACK_PARAMETERS     @ "callbackParameters"
#define KEY_PARTNER_PARAMETERS      @ "partnerParameters"
#define KEY_IS_RECEIPT_SET          @ "isReceiptSet"
#define KEY_USER_AGENT              @ "userAgent"
#define KEY_REFERRER                @ "referrer"
#define KEY_SHOULD_LAUNCH_DEEPLINK  @ "shouldLaunchDeeplink"
#define KEY_SEND_IN_BACKGROUND      @ "sendInBackground"
#define KEY_DELAY_START             @ "delayStart"

@implementation AdjustCordova {
    NSString *attributionCallbackId;
    NSString *eventSuccessfulCallbackId;
    NSString *eventFailureCallbackId;
    NSString *sessionSuccessfulCallbackId;
    NSString *sessionFailureCallbackId;
    NSString *deeplinkCallbackId;
    BOOL _shouldLaunchDeeplink;
}

- (void)pluginInitialize {
    attributionCallbackId = nil;
    eventSuccessfulCallbackId = nil;
    eventFailureCallbackId = nil;
    sessionSuccessfulCallbackId = nil;
    sessionFailureCallbackId = nil;
    deeplinkCallbackId = nil;
}

- (void)adjustAttributionChanged:(ADJAttribution *)attribution {
    NSDictionary *attributionDictionary = [attribution dictionary];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:attributionDictionary];
    pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:attributionCallbackId];
}

- (void)adjustEventTrackingSucceeded:(ADJEventSuccess *)eventSuccessResponseData {
    NSDictionary *dict = [eventSuccessResponseData dictionary];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dict];
    pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:eventSuccessfulCallbackId];
}

- (void)adjustEventTrackingFailed:(ADJEventFailure *)eventFailureResponseData {
    NSDictionary *dict = [eventFailureResponseData dictionary];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dict];
    pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:eventFailureCallbackId];
}

- (void)adjustSessionTrackingSucceeded:(ADJSessionSuccess *)sessionSuccessResponseData {
    NSDictionary *dict = [sessionSuccessResponseData dictionary];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dict];
    pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:sessionSuccessfulCallbackId];
}

- (void)adjustSessionTrackingFailed:(ADJSessionFailure *)sessionFailureResponseData {
    NSDictionary *dict = [sessionFailureResponseData dictionary];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dict];
    pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:sessionFailureCallbackId];
}

- (BOOL)adjustDeeplinkResponse:(NSURL *)deeplink {
    //NSMutableDictionary *dict = [NSMutableDictionary new];
    //NSString *path = [[NSString alloc] initWithString:[deeplink path]];
    //[dict setObject:path forKey:@"deeplink"];

    //CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dict];
    //pluginResult.keepCallback = [NSNumber numberWithBool:YES];

    //[self.commandDelegate sendPluginResult:pluginResult callbackId:attributionCallbackId];
    return _shouldLaunchDeeplink;
}

- (void)create:(CDVInvokedUrlCommand *)command {
    NSString *appToken = [[command.arguments objectAtIndex:0] objectForKey:KEY_APP_TOKEN];
    NSString *environment = [[command.arguments objectAtIndex:0] objectForKey:KEY_ENVIRONMENT];
    NSString *logLevel = [[command.arguments objectAtIndex:0] objectForKey:KEY_LOG_LEVEL];
    NSString *sdkPrefix = [[command.arguments objectAtIndex:0] objectForKey:KEY_SDK_PREFIX];
    NSString *defaultTracker = [[command.arguments objectAtIndex:0] objectForKey:KEY_DEFAULT_TRACKER];
    NSNumber *eventBufferingEnabled = [[command.arguments objectAtIndex:0] objectForKey:KEY_EVENT_BUFFERING_ENABLED];

    BOOL sendInBackground = [[[command.arguments objectAtIndex:0] objectForKey:KEY_SEND_IN_BACKGROUND] boolValue];
    BOOL shouldLaunchDeeplink = [[[command.arguments objectAtIndex:0] objectForKey:KEY_SHOULD_LAUNCH_DEEPLINK] boolValue];

    NSString *userAgent = [[command.arguments objectAtIndex:0] objectForKey:KEY_USER_AGENT];
    double delayStart = [[[command.arguments objectAtIndex:0] objectForKey:KEY_DELAY_START] doubleValue];

    ADJConfig *adjustConfig = [ADJConfig configWithAppToken:appToken environment:environment];

    if ([adjustConfig isValid]) {
        // Log level
        if ([self isFieldValid:logLevel]) {
     [adjustConfig setLogLevel:[ADJLogger LogLevelFromString:[logLevel lowercaseString]]];
        }

        // Event buffering
        if ([self isFieldValid:eventBufferingEnabled]) {
            [adjustConfig setEventBufferingEnabled:[eventBufferingEnabled boolValue]];
        }

        // SDK prefix
        if ([self isFieldValid:sdkPrefix]) {
            [adjustConfig setSdkPrefix:sdkPrefix];
        }

        // Default tracker
        if ([self isFieldValid:defaultTracker]) {
            [adjustConfig setDefaultTracker:defaultTracker];
        }

        // Attribution delegate
        if (attributionCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // event successful delegate
        if (eventSuccessfulCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // event failure delegate
        if (eventFailureCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // session successful delegate
        if (sessionSuccessfulCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // session failure delegate
        if (sessionFailureCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // deeplink delegate
        if (deeplinkCallbackId != nil) {
            [adjustConfig setDelegate:self];
        }

        // send in background
        [adjustConfig setSendInBackground:sendInBackground];

        // should launch deeplink
        _shouldLaunchDeeplink = shouldLaunchDeeplink;

        // User agent
        if ([self isFieldValid:userAgent]) {
            [adjustConfig setUserAgent:userAgent];
        }

        // delayStart
        [adjustConfig setDelayStart:delayStart];

        [Adjust appDidLaunch:adjustConfig];
    }
}

- (void)trackEvent:(CDVInvokedUrlCommand *)command {
    NSString *eventToken = [[command.arguments objectAtIndex:0] objectForKey:KEY_EVENT_TOKEN];
    NSString *revenue = [[command.arguments objectAtIndex:0] objectForKey:KEY_REVENUE];
    NSString *currency = [[command.arguments objectAtIndex:0] objectForKey:KEY_CURRENCY];
    NSString *receipt = [[command.arguments objectAtIndex:0] objectForKey:KEY_RECEIPT];
    NSString *transactionId = [[command.arguments objectAtIndex:0] objectForKey:KEY_TRANSACTION_ID];
    NSNumber *isReceiptSet = [[command.arguments objectAtIndex:0] objectForKey:KEY_IS_RECEIPT_SET];

    NSMutableArray *callbackParameters = [[NSMutableArray alloc] init];
    NSMutableArray *partnerParameters = [[NSMutableArray alloc] init];

    for (id item in [[command.arguments objectAtIndex:0] objectForKey:KEY_CALLBACK_PARAMETERS]) {
                        [callbackParameters addObject:item];
    }

    for (id item in [[command.arguments objectAtIndex:0] objectForKey:KEY_PARTNER_PARAMETERS]) {
                         [partnerParameters addObject:item];
    }

    ADJEvent *adjustEvent = [ADJEvent eventWithEventToken:eventToken];

    if ([adjustEvent isValid]) {
        if ([self isFieldValid:revenue]) {
            double revenueValue = [revenue doubleValue];

            [adjustEvent setRevenue:revenueValue currency:currency];
        }

        for (int i = 0; i < [callbackParameters count]; i += 2) {
            NSString *key = [callbackParameters objectAtIndex:i];
            NSString *value = [callbackParameters objectAtIndex:(i+1)];

            [adjustEvent addCallbackParameter:key value:value];
        }

        for (int i = 0; i < [partnerParameters count]; i += 2) {
            NSString *key = [partnerParameters objectAtIndex:i];
            NSString *value = [partnerParameters objectAtIndex:(i+1)];

            [adjustEvent addPartnerParameter:key value:value];
        }

        BOOL isTransactionIdSet = false;

        if ([self isFieldValid:isReceiptSet]) {
            if ([isReceiptSet boolValue]) {
                [adjustEvent setReceipt:[receipt dataUsingEncoding:NSUTF8StringEncoding] transactionId:transactionId];
            } else {
                if ([self isFieldValid:transactionId]) {
         [adjustEvent setTransactionId:transactionId];

         isTransactionIdSet = YES;
                }
            }
        }

        if (NO == isTransactionIdSet) {
            if ([self isFieldValid:transactionId]) {
     [adjustEvent setTransactionId:transactionId];
            }
        }

        [Adjust trackEvent:adjustEvent];
    }
}

- (void)setOfflineMode:(CDVInvokedUrlCommand *)command {
    NSNumber *isEnabledNumber = [command argumentAtIndex:0 withDefault:nil];

    if (isEnabledNumber == nil) {
        return;
    }

    [Adjust setOfflineMode:[isEnabledNumber boolValue]];
}

- (void)appWillOpenUrl:(CDVInvokedUrlCommand *)command {
    NSString *urlString = [command argumentAtIndex:0 withDefault:nil];

    if (urlString == nil) {
        return;
    }

    NSURL *url = [NSURL URLWithString:[urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];

    [Adjust appWillOpenUrl:url];
}

- (void)getIdfa:(CDVInvokedUrlCommand *)command {
    NSString *idfa = [Adjust idfa];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:idfa];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)setEnabled:(CDVInvokedUrlCommand *)command {
    NSNumber *isEnabledNumber = [command argumentAtIndex:0 withDefault:nil];

    if (isEnabledNumber == nil) {
        return;
    }

    [Adjust setEnabled:[isEnabledNumber boolValue]];
}

- (void)isEnabled:(CDVInvokedUrlCommand *)command {
    BOOL isEnabled = [Adjust isEnabled];

    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsBool:isEnabled];

    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)onPause:(CDVInvokedUrlCommand *)command {

}

- (void)onResume:(CDVInvokedUrlCommand *)command {

}

- (void)getGoogleAdId:(CDVInvokedUrlCommand *)command {

}

- (void)sendFirstPackages:(CDVInvokedUrlCommand *)command {
    [Adjust sendFirstPackages];
}

- (void)setAttributionCallback:(CDVInvokedUrlCommand *)command {
    attributionCallbackId = command.callbackId;
}

- (void)setEventTrackingSuccessfulCallback:(CDVInvokedUrlCommand *)command {
    eventSuccessfulCallbackId = command.callbackId;
}

- (void)setEventTrackingFailureCallback:(CDVInvokedUrlCommand *)command {
    eventFailureCallbackId = command.callbackId;
}

- (void)setSessionTrackingSuccessfulCallback:(CDVInvokedUrlCommand *)command {
    sessionSuccessfulCallbackId = command.callbackId;
}

- (void)setSessionTrackingFailureCallback:(CDVInvokedUrlCommand *)command {
    sessionFailureCallbackId = command.callbackId;
}

- (void)setDeeplinkCallback:(CDVInvokedUrlCommand *)command {
    deeplinkCallbackId = command.callbackId;
}

- (void)addSessionCallbackParameter:(CDVInvokedUrlCommand *)command {
    NSString *key = [command argumentAtIndex:0 withDefault:nil];
    NSString *value = [command argumentAtIndex:1 withDefault:nil];

    if (!([self isFieldValid:key]) || !([self isFieldValid:value]) ) {
    return;
    }

    [Adjust addSessionCallbackParameter:key value:value];
}

- (void)removeSessionCallbackParameter:(CDVInvokedUrlCommand *)command {
    NSString *key = [command argumentAtIndex:0 withDefault:nil];

    if (!([self isFieldValid:key])) {
    return;
    }

    [Adjust removeSessionCallbackParameter:key];
}

- (void)resetSessionCallbackParameters:(CDVInvokedUrlCommand *)command {
    [Adjust resetSessionCallbackParameters];
}

- (void)addSessionPartnerParameter:(CDVInvokedUrlCommand *)command {
    NSString *key = [command argumentAtIndex:0 withDefault:nil];
    NSString *value = [command argumentAtIndex:1 withDefault:nil];

    if (!([self isFieldValid:key]) || !([self isFieldValid:value]) ) {
    return;
    }

    [Adjust addSessionPartnerParameter:key value:value];
}

- (void)removeSessionPartnerParameter:(CDVInvokedUrlCommand *)command {
    NSString *key = [command argumentAtIndex:0 withDefault:nil];

    if (!([self isFieldValid:key])) {
    return;
    }

    [Adjust removeSessionPartnerParameter:key];
}

- (void)resetSessionPartnerParameters:(CDVInvokedUrlCommand *)command {
    [Adjust resetSessionPartnerParameters];
}

- (BOOL)isFieldValid:(NSObject *)field {
    if (![field isKindOfClass:[NSNull class]]) {
        if (field != nil) {
            return YES;
        }
    }

    return NO;
}

@end
