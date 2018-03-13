//
//  StreamPowerPrivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/11.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

typedef struct {
    XRTimeRange range;
    UInt64 idx;
    UInt32 recno;
} XRPowerTimelineEntry;

@interface XRPowerTimeline : NSObject
- (UInt64)count;
- (UInt64)lastIndex;
- (XRTime)lastTimeOffset;
- (void)enumerateTimeRange:(XRTimeRange)timeRange sequenceNumberRange:(NSRange)numberRange block:(void (^)(const XRPowerTimelineEntry *entry, BOOL *stop))block;
@end

@interface XRPowerStreamDefinition : NSObject
- (UInt64)columnsInDataStreamCount;
@end

@interface XRPowerDatum : NSObject
- (XRTimeRange)time;
- (NSString *)labelForColumn:(SInt64)column;
- (NSString *)objectValueForColumn:(SInt64)column;
- (NSString *)stringValueForColumn:(SInt64)column;
- (NSString *)stringForTimeColumn:(XRTimeRange)time;
@end

@interface XRPowerDetailController : NSObject
- (XRPowerDatum *)datumAtObjectIndex:(UInt64)index;
@end

@interface XRStreamedPowerInstrument : XRLegacyInstrument {
    XRPowerDetailController *_detailController;
    NSMutableDictionary *_eventStreamParsers;
    NSMutableDictionary *_measStreamParsers;
    NSMutableDictionary *_stateStreamParsers;
    NSMutableSet *_streamIDs;
    NSMutableDictionary *_streamIDsByHandle;
    unsigned long long _detailViewType;
    unsigned long long _myLastTickTime;
}
- (XRPowerStreamDefinition *)definitionForCurrentDetailView;
- (XRPowerTimeline *)selectedEventTimeline;
@end


//解析 CPU Activity / Energy Activity / Network Activity等非标准 Instruments 模板
void ParsePowerMobileTemplate(id<XRContextContainer>container){
    XRStreamedPowerInstrument *Instrument = (XRStreamedPowerInstrument *)container;
    [Instrument._permittedContexts[0] display];
    
    
    NSMutableDictionary *viewingPreferences = LKIvar(LKIvar(LKIvar(Instrument, _detailController), _tableView),_viewingPreferences);
    UInt64 columnCount = viewingPreferences.count - 1;
    UInt64 rowCount = Instrument.selectedEventTimeline.count;
    BOOL printTitle = YES;
    
    XRPowerDetailController *powerDetail = LKIvar(Instrument, _detailController);
    for (UInt64 row = 0; row < rowCount; row++) {
        XRPowerDatum *datum = [powerDetail datumAtObjectIndex:row];
        NSMutableString *string = [NSMutableString string];
        NSMutableString *title = [NSMutableString string];
        [string appendFormat:@"#%@, ",[datum stringForTimeColumn:datum.time]];
        [title appendFormat:@"time, "];
        for (UInt64 column = 0; column < columnCount; column++) {
            if (column > 0) {
                [string appendString:@", "];
                [title appendFormat:@", "];
            }
            [string appendFormat:@"%@",[datum stringValueForColumn:column]];
            [title appendFormat:@"%@", [datum labelForColumn:column]];
        }
        if (printTitle) {
            LKPrint(@"%@\n", title);
            printTitle = NO;
        }
        LKPrint(@"%@\n", string);
    }
}
