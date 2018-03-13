//
//  NetworkingPrivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/11.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

@interface XRNetworkAddressFormatter : NSFormatter
@end

@interface XRNetworkingInstrument : XRLegacyInstrument {
    XRContext * __strong *_topLevelContexts;
    NSArrayController * __strong *_controllersByTable;
    XRNetworkAddressFormatter *_localAddrFmtr;
    XRNetworkAddressFormatter *_remoteAddrFmtr;
}
- (void)selectedRunRecomputeSummaries;
@end


//解析 Networking
void ParseNetworking(id<XRContextContainer>container){
    // Connections: print out all connections.
    XRNetworkingInstrument *networkingInstrument = (XRNetworkingInstrument *)container;
    [LKIvarCast(networkingInstrument, _topLevelContexts, XRContext * const *)[1] display]; // 3 contexts: Processes, Connections, Interfaces.
    [networkingInstrument selectedRunRecomputeSummaries];
    NSArrayController *arrayController = LKIvarCast(networkingInstrument, _controllersByTable, NSArrayController * const *)[1]; // The same index as for contexts.
    XRNetworkAddressFormatter *localAddressFormatter = LKIvar(networkingInstrument, _localAddrFmtr);
    XRNetworkAddressFormatter *remoteAddressFormatter = LKIvar(networkingInstrument, _remoteAddrFmtr);
    NSByteCountFormatter *byteFormatter = [[NSByteCountFormatter alloc]init];
    byteFormatter.countStyle = NSByteCountFormatterCountStyleBinary;
    for (NSDictionary *entry in arrayController.arrangedObjects) {
        NSString *localAddress = [localAddressFormatter stringForObjectValue:entry[@"localAddr"]];
        NSString *remoteAddress = [remoteAddressFormatter stringForObjectValue:entry[@"remoteAddr"]];
        NSString *inSize = [byteFormatter stringForObjectValue:entry[@"totalRxBytes"]];
        NSString *outSize = [byteFormatter stringForObjectValue:entry[@"totalTxBytes"]];
        LKPrint(@"%@ -> %@: %@ received, %@ sent\n", localAddress, remoteAddress, inSize, outSize);
    }
}
