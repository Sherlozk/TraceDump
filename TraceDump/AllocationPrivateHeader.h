//
//  AllocationParivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/9.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"


@interface XRRawBacktrace : NSObject
@end

@interface XRManagedEvent : NSObject
- (UInt32)identifier;
@end

@interface XRObjectAllocEvent : XRManagedEvent
- (UInt32)allocationEvent;
- (UInt32)destructionEvent;
- (UInt32)pastEvent;
- (UInt32)futureEvent;
- (BOOL)isAliveThroughIdentifier:(UInt32)identifier;
- (NSString *)eventTypeName;
- (NSString *)categoryName;
- (XRTime)timestamp; // Time elapsed from the beginning of the run.
- (SInt32)size; // in bytes
- (SInt32)delta; // in bytes
- (UInt64)address;
- (UInt64)slot;
- (UInt64)data;
- (XRRawBacktrace *)backtrace;
@end

@interface XRObjectAllocEventViewController : NSObject {
    XRManagedEventArrayController *_ac;
}
@end

@interface XRObjectAllocInstrument : XRLegacyInstrument {
    XRObjectAllocEventViewController *_objectListController;
}
- (NSArray<XRContext *> *)_topLevelContexts;
@end

@interface XROAEventSummary : NSObject
@end

@interface XRObjectAllocRun : XRRun
- ( XRTimeRange)_displayTimeFilter;
- ( XRTimeRange)selectedTimeRange;
- (void)setSelectedTimeRange:( XRTimeRange)timeRange;
- (NSArray *)globalStats;
- (XROAEventSummary *)scalingStats;
- (void)refreshStatsForActiveTimeFilter;
- (unsigned long long)lastTimestamp;
@end

//解析 allocation
void ParseAllocation(id<XRContextContainer>container){
    XRObjectAllocInstrument *allocInstrument = (XRObjectAllocInstrument *)container;
    [allocInstrument._topLevelContexts[2] display]; // 4 contexts: Statistics, Call Trees, Allocations List, Generations.
    XRManagedEventArrayController *arrayController = LKIvar(LKIvar(allocInstrument, _objectListController), _ac);
    NSMutableDictionary<NSNumber *, NSNumber *> *sizeGroupedByTime = [NSMutableDictionary dictionary];
    int i = 0;
    for (XRObjectAllocEvent *event in arrayController.arrangedObjects) {
//        LKPrint(@"timestamp:%lld",event.timestamp);
        if (event.size != event.delta) {
            LKPrint(@"xxx");
        }
        NSNumber *time = @(event.timestamp /( NSEC_PER_SEC));
        NSNumber *size = @(sizeGroupedByTime[time].integerValue + event.size);
        sizeGroupedByTime[time] = size;
        i++;
    }
    LKPrint(@"i = %d",i);
    NSArray<NSNumber *> *sortedTime = [sizeGroupedByTime.allKeys sortedArrayUsingComparator:^(NSNumber *time1, NSNumber *time2) {
        return [sizeGroupedByTime[time2] compare:sizeGroupedByTime[time1]];
    }];
    NSByteCountFormatter *byteFormatter = [[NSByteCountFormatter alloc]init];
    byteFormatter.countStyle = NSByteCountFormatterCountStyleBinary;
    LKPrint(@"time, memory size");
    for (NSNumber *time in sortedTime) {
        NSString *size = [byteFormatter stringForObjectValue:sizeGroupedByTime[time]];
        LKPrint(@"#%@, %@\n", time, size);
    }
}



void ParseAllocationWithRun(XRRun *run){
    NSByteCountFormatter *byteFormatter = [[NSByteCountFormatter alloc]init];
    byteFormatter.countStyle = NSByteCountFormatterCountStyleBinary;
    XRObjectAllocRun *OARun = (XRObjectAllocRun *)run;
    
    LKPrint(@"time, persisentBytes");
    long long lastTimeStamp = [OARun lastTimestamp];
    
    for (long long i = 0; i*NSEC_PER_SEC<=lastTimeStamp; i++) {
        XRTimeRange select;
        select.start = 0;
        select.length = i*NSEC_PER_SEC;
        [OARun setSelectedTimeRange:select];
        [OARun refreshStatsForActiveTimeFilter];
        NSArray *globalStats = [OARun globalStats];
        XROAEventSummary *allHeapVM = globalStats[1];
        long long allHrapVM_ab = LKIvarCast(allHeapVM, activeBytes, long long);
        NSString *persisentBytesStr = [byteFormatter stringForObjectValue:[NSNumber numberWithLongLong:allHrapVM_ab]];
        if ([persisentBytesStr hasPrefix:@"Zero"]) {
            persisentBytesStr = @"0 KB";
        }
        LKPrint(@"#%lld, %@", i, persisentBytesStr);
    }
}

