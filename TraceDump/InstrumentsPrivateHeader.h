//
//  InstrumentsPrivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/9.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//



@interface DVTPlugInManager : NSObject
+ (id)defaultPlugInManager;
- (void)_preLoadPlugIns;
@end

@interface XRActionContextManager : NSObject
{
    NSMutableArray *_actions;
    NSMutableDictionary *_classTargetedActions;
    NSMutableDictionary *_protocolTargetActions;
}
+ (id)sharedContextManager;
+ (void)initializePlugIns:(id)arg1;
@end


@interface DVTDeveloperPaths : NSObject
+ (NSString *)applicationDirectoryName;
+ (void)initializeApplicationDirectoryName:(NSString *)name;
@end

@interface XRInternalizedSettingsStore : NSObject
+ (NSDictionary *)internalizedSettings;
+ (void)configureWithAdditionalURLs:(NSArray *)urls;
@end

typedef UInt64 XRTime; // in nanoseconds
typedef struct { XRTime start, length; } XRTimeRange;

@interface XRRun : NSObject
- (SInt64)runNumber;
- (NSString *)displayName;
- (XRTimeRange)timeRange;
@end

@interface PFTInstrumentType : NSObject
- (NSString *)uuid;
- (NSString *)name;
- (NSString *)category;
@end

@protocol XRInstrumentViewController;

@protocol XRContextContainer;

@interface XRContext : NSObject
- (NSString *)label;
- (id<NSCoding>)value;
- (id<XRContextContainer>)container;
- (instancetype)parentContext;
- (instancetype)rootContext;
- (void)display;
@end

@interface XRInstrument : NSObject <XRContextContainer>
- (PFTInstrumentType *)type;
- (id<XRInstrumentViewController>)viewController;
- (void)setViewController:(id<XRInstrumentViewController>)viewController;
- (NSArray<XRRun *> *)allRuns;
- (XRRun *)currentRun;
- (void)setCurrentRun:(XRRun *)run;
- (XRContext *)topLevelCommonContexts;
- (XRContext *)splitterContext;
- (XRContext *)summaryViewContext;
- (XRContext *)sourceViewContext;
- (XRContext *)scriptViewContext;
- (XRContext *)consoleViewContext;
- (XRContext *)_permittedContexts;
@end

@interface PFTInstrumentList : NSObject
- (NSArray<XRInstrument *> *)allInstruments;
@end

@interface XRTrace : NSObject
- (PFTInstrumentList *)allInstrumentsList;
@end

@interface XRDevice : NSObject
- (NSString *)deviceIdentifier;
- (NSString *)deviceDisplayName;
- (NSString *)deviceDescription;
- (NSString *)productType;
- (NSString *)productVersion;
- (NSString *)buildVersion;
@end

@interface PFTProcess : NSObject
- (NSString *)bundleIdentifier;
- (NSString *)processName;
- (NSString *)displayName;
@end

@interface DTTimelineGraph : NSView
@end

@interface XRTimelineGraph : DTTimelineGraph
@end

@interface PFTWindowController : NSWindowController
@property(retain) XRTimelineGraph *timelineGraph;
- (void)setDocument:(id)document;
-(void)setupTrackPinningGraph;
-(void)windowDidLoad;
-(void)loadWindow;
@end

@interface PFTTraceDocument : NSDocument
- (XRTrace *)trace;
- (XRDevice *)targetDevice;
- (PFTProcess *)defaultProcess;
- (PFTWindowController *)mainWindowController;

@end



@interface PFTDocumentController : NSDocumentController
+ (id)importData:(id)arg1 error:(id *)arg2;
@end



@protocol XRContextContainer <NSObject>
- (XRContext *)contextRepresentation;
- (void)displayContext:(XRContext *)context;
@end

@protocol XRFilteredDataSource <NSObject>
@end

@protocol XRAnalysisCoreViewSubcontroller <XRContextContainer, XRFilteredDataSource>
@end



@protocol XRInstrumentViewController <NSObject>
- (id<XRContextContainer>)detailContextContainer;
- (id<XRFilteredDataSource>)detailFilteredDataSource;
- (void)instrumentDidChangeSwitches;
- (void)instrumentChangedTableRequirements;
- (void)instrumentWillBecomeInvalid;
@end

@interface PFTCallTreeNode : NSObject
@end

@interface XRBacktraceRepository : NSObject
- (PFTCallTreeNode *)rootNode;
@end

@interface XRAnalysisCoreStandardController : NSObject <XRInstrumentViewController>
- (instancetype)initWithInstrument:(XRInstrument *)instrument document:(PFTTraceDocument *)document;
- (XRBacktraceRepository *)backtraceRepository;
@end





@interface XRMultiProcessBacktraceRepository : XRBacktraceRepository
@end

@interface XRManagedEventArrayController : NSArrayController
@end

@interface XRCallTreeDetailView : NSView
- (XRBacktraceRepository *)backtraceRepository;
@end

@interface XRLegacyInstrument : XRInstrument <XRInstrumentViewController, XRContextContainer>
- (NSArray<XRContext *> *)_permittedContexts;
@end
