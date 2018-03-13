//
//  TimeProfilePrivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/9.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

@interface XRAnalysisCoreDetailViewController : NSViewController <XRAnalysisCoreViewSubcontroller>
- (XRContext *)searchContexts;
- (XRContext *)currentDataContext;
- (void)displayContext:(XRContext *)context;
- (XRContext *)contextRepresentation;
@end

//解析 time profile
//void ParseTimeProfile(XRRun *run){
//    XRSamplerRun *samplerRun = (XRSamplerRun *)run;
//    NSLog( @"ssss");
//
//}

void ParseTimeProfile(id<XRContextContainer>container){
//    XRAnalysisCoreDetailViewController *controller = (XRAnalysisCoreDetailViewController *)container ;
//    XRCallTreeDetailView *callTreeView = LKIvar(LKIvar(controller, _callTreeViewController),_callTreeView);
//    XRBacktraceRepository *backtraceRepository = callTreeView.backtraceRepository;
//    LKPrint(@"ss");
}


//void ParseTimeProfile(id<XRContextContainer>container){
//    // Time Profiler: print out all functions in descending order of self execution time.
//    XRCallTreeDetailView *callTreeView = (XRCallTreeDetailView *)container;
//    XRBacktraceRepository *backtraceRepository = callTreeView.backtraceRepository;
//    static NSMutableArray<PFTCallTreeNode *> * (^ const flattenTree)(PFTCallTreeNode *) = ^(PFTCallTreeNode *rootNode) { // Helper function to collect all tree nodes.
//        NSMutableArray *nodes = [NSMutableArray array];
//        if (rootNode) {
//            [nodes addObject:rootNode];
//            for (PFTCallTreeNode *node in rootNode.children) {
//                [nodes addObjectsFromArray:flattenTree(node)];
//            }
//        }
//        return nodes;
//    };
//    NSMutableArray<PFTCallTreeNode *> *nodes = flattenTree(backtraceRepository.rootNode);
//    [nodes sortUsingDescriptors:@[[NSSortDescriptor sortDescriptorWithKey:NSStringFromSelector(@selector(terminals)) ascending:NO]]];
//    for (PFTCallTreeNode *node in nodes) {
//        LKPrint(@"%@ %@ %i ms\n", node.libraryName, node.symbolName, node.terminals);
//    }
//}

