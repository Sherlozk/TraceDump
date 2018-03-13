//
//  main.m
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/8.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PrivateCPlusPlusHeader.h"
#import "InstrumentsPrivateHeader.h"
#import "AllocationPrivateHeader.h"
#import "TimeProfilePrivateHeader.h"
#import "VideoCardRunPrivateHeader.h"
#import "StreamPowerPrivateHeader.h"
#import "NetworkingPrivateHeader.h"
#import "ActivityMonitorHeader.h"




static NSBundle *(*NSBundle_mainBundle_original)(id self, SEL _cmd);
static NSBundle *NSBundle_mainBundle_replaced(id self, SEL _cmd) {
    return [NSBundle bundleWithPath:@"/Applications/Xcode.app/Contents/Applications/Instruments.app"];
//    return [NSBundle bundleWithPath:@"/Users/sherlock/Downloads/Xcode-beta.app/Contents/Applications/Instruments.app"];
}

static void __attribute__((constructor)) hook() {
    Method NSBundle_mainBundle = class_getClassMethod(NSBundle.class, @selector(mainBundle));
    NSBundle_mainBundle_original = (void *)method_getImplementation(NSBundle_mainBundle);
    method_setImplementation(NSBundle_mainBundle, (IMP)NSBundle_mainBundle_replaced);
}


int main(int argc, const char * argv[]) {
    if (argc !=2) {
        LKPrint(@"usage:  /path/to/TraceDump /path/to/xxx.trace");
        return 0;
    }
    @autoreleasepool {
        //初始化 Instruments
        DVTInitializeSharedFrameworks();
        [DVTDeveloperPaths initializeApplicationDirectoryName:@"Instruments"];
        [XRInternalizedSettingsStore configureWithAdditionalURLs:nil];
        PFTLoadPlugins();
        
        [PFTDocumentController sharedDocumentController];
        
        // 打开一个 trace document.
//        NSString *tracePath = @"/Users/sherlock/Desktop/activity_monitor.trace";
//        NSString *tracePath = @"/Users/sherlock/腾讯视频云/InstrumentsRB.trace";
        NSString *tracePath = [[NSString alloc]initWithUTF8String:argv[1]];
        NSError *error = nil;
        NSURL *traceUrl = [NSURL fileURLWithPath:tracePath];
        
        //iOS11之后需要删除trace文件里面的open.creq文件才能正常读取
        NSFileManager *fileManager = [NSFileManager defaultManager];
        [fileManager removeItemAtPath:[tracePath stringByAppendingString:@"/open.creq"] error:&error];
        if (error) {
            LKPrint(@"\nWarning: %@\n", error);
        }else{
            LKPrint(@"delete open.creq success!");
        }
        
        //读取trace文件
        error = nil;
        PFTTraceDocument *document = [[PFTTraceDocument alloc]initWithContentsOfURL:traceUrl ofType:@"Trace Document" error:&error];
//        [document makeWindowControllers];
//        PFTWindowController *window = document.mainWindowController;
////        [window setDocument:document];
//        [window loadWindow];
//        [window windowDidLoad];
        
        if (error) {
            LKPrint(@"\nError: %@\n", error);
            return 1;
        }
        LKPrint(@"Trace: %@\n", tracePath);
        
        // List some useful metadata of the document.
        XRDevice *device = document.targetDevice;
        LKPrint(@"Device: %@ (%@ %@ %@)\n", device.deviceDisplayName, device.productType, device.productVersion, device.buildVersion);
        PFTProcess *process = document.defaultProcess;
        LKPrint(@"Process: %@ (%@)\n", process.displayName, process.bundleIdentifier);
        
        
        // Each trace document consists of data from several different instruments.
        XRTrace *trace = document.trace;
        //打印Target信息
        NSDictionary *_runData = LKIvar(LKIvar(trace, _runData),_runData)[@1];
        NSArray *arr = _runData[@"recordingSettingsSummary"];
        LKPrint(@"%@",arr[0]);
        for (XRInstrument *instrument in trace.allInstrumentsList.allInstruments) {
            LKPrint(@"\nInstrument: %@ (%@)\n", instrument.type.name, instrument.type.uuid);
            
            // Common routine to obtain the data container.
            if (![instrument isKindOfClass:XRLegacyInstrument.class]) {
                instrument.viewController = [[XRAnalysisCoreStandardController alloc]initWithInstrument:instrument document:document];
            }
            id<XRInstrumentViewController> controller = instrument.viewController;
            [controller instrumentDidChangeSwitches];
            [controller instrumentChangedTableRequirements];
//            XRContext *context = controller.detailContextContainer.contextRepresentation;
//            [context display];
            id<XRContextContainer> container = controller.detailContextContainer.contextRepresentation.container;
            
            // Each instrument can have multiple runs.
            NSArray<XRRun *> *runs = instrument.allRuns;
            if (runs.count == 0) {
                LKPrint(@"No data.\n");
                continue;
            }
            
            for (XRRun *run in runs) {
                LKPrint(@"Run #%@: %@\n", @(run.runNumber), run.displayName);
                instrument.currentRun = run;
                NSString *instrumentID = instrument.type.uuid;
                
                if ([instrumentID isEqualToString:@"com.apple.xray.instrument-type.coresampler2"]) {
                    ParseTimeProfile(instrument);
                }else if ([instrumentID isEqualToString:@"com.apple.xray.instrument-type.oa"]) {
                    ParseAllocationWithRun(run);
//                    ParseAllocation(container);
                }else if ([instrumentID isEqualToString:@"com.apple.xray.instrument-type.coreanimation"]){
                    ParseCoreAnimation(run);
                }else if([instrumentID isEqualToString:@"com.apple.xray.power.mobile.cpu"] || [instrumentID isEqualToString:@"com.apple.xray.power.mobile.energy"] || [instrumentID isEqualToString:@"com.apple.xray.power.mobile.net"]){
                    ParsePowerMobileTemplate(container);
                }else if([instrumentID isEqualToString:@"com.apple.xray.instrument-type.quartz"]){
                    ParseGPUDriver(run);
                }else if ([instrumentID isEqualToString:@"com.apple.xray.instrument-type.networking"]){
                    ParseNetworking(container);
                }else if([instrumentID isEqualToString:@"com.apple.xray.instrument-type.activity.all"]){
                    ParseActivitymonitor(run);
                }else {
                    LKPrint(@"Data processor has not been implemented for this type of instrument.\n");
                }
                
            }
            [controller instrumentWillBecomeInvalid];
        }
        [document close];
    }
    return 0;
}
