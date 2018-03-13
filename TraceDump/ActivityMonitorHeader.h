//
//  ActivityMonitorHeader.h
//  TraceDump
//
//  Created by sherlock on 06/03/2018.
//  Copyright © 2018 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

@interface XRActivityInstrumentRun : XRRun{
    NSMutableArray *_data;
    XRBacktraceRepository *_primaryRepository;
    NSArrayController *_controller;
}

@end


void ParseActivitymonitor(XRRun *run){
    XRActivityInstrumentRun *activityRun = (XRActivityInstrumentRun *)run;
    NSMutableArray *_data = LKIvar(activityRun, _data);
    LKPrint(@"time, CPUUserLoad, PhysicalMemoryActive, PhysicalMemoryWired, PhysicalMemoryUsed");
    for (NSDictionary *sample in _data) {
        UInt64 timestamp = ([sample[@"XRActivityClientTraceRelativeTimestamp"] integerValue] / USEC_PER_SEC)/1000;
        NSNumber *cpuUserLoad = sample[@"CPUUserLoad"];
        NSNumber *PhysicalMemoryActive = sample[@"PhysicalMemoryActive"];
        NSNumber *PhysicalMemoryWired = sample[@"PhysicalMemoryWired"];
        NSNumber *PhysicalMemoryUsed = sample[@"PhysicalMemoryUsed"];
        if(cpuUserLoad == nil){
            cpuUserLoad = [[NSNumber alloc]initWithFloat:0];
        }
        if (PhysicalMemoryActive == nil) {
            PhysicalMemoryActive = [[NSNumber alloc]initWithFloat:0];
        }
        NSByteCountFormatter *BCFromatter = [[NSByteCountFormatter alloc]init];
        BCFromatter.countStyle = NSByteCountFormatterCountStyleMemory;
        
        LKPrint(@"#%@, %@, %@, %@, %@\n", @(timestamp), cpuUserLoad, [BCFromatter stringForObjectValue:PhysicalMemoryActive], [BCFromatter stringForObjectValue:PhysicalMemoryWired], [BCFromatter stringForObjectValue:PhysicalMemoryUsed]);
    }
}
