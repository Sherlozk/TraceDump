//
//  CoreAnimationPrivateHeader.h
//  TraceDump
//
//  Created by 张恒瑜 on 2018/2/9.
//  Copyright © 2018年 张恒瑜. All rights reserved.
//

#import "InstrumentsPrivateHeader.h"

@interface XRVideoCardRun : XRRun {
    NSArrayController *_controller;
}
@end

//解析 core animation
void ParseCoreAnimation(XRRun *run){
    // Core Animation: print out all FPS data samples.
    XRVideoCardRun *videoCardRun = (XRVideoCardRun *)run;
    NSArrayController *arrayController = LKIvar(videoCardRun, _controller);
    LKPrint(@"time, FPS");
    for (NSDictionary *sample in arrayController.arrangedObjects) {
        NSNumber *fps = sample[@"FramesPerSecond"];
        UInt64 timestamp = [sample[@"XRVideoCardRunTimeStamp"] integerValue] / USEC_PER_SEC;
        LKPrint(@"#%@, %@\n", @(timestamp), fps);
    }
}

//解析 GPU Driver
void ParseGPUDriver(XRRun *run){
    XRVideoCardRun *videoCardRun = (XRVideoCardRun *)run;
    NSArrayController *arrayController = LKIvar(videoCardRun, _controller);
    LKPrint(@"time, Device Utilization, Renderer Utilization, Tiler Utilization, textureCount");
    for (NSDictionary *sample in arrayController.arrangedObjects) {
        NSNumber *deviceUti = sample[@"Device Utilization %"];
        NSNumber *tilerUti = sample[@"Tiler Utilization %"];
        NSNumber *rendererUti = sample[@"Renderer Utilization %"];
        NSNumber *textureCount = sample[@"textureCount"];
        UInt64 timestamp = [sample[@"XRVideoCardRunTimeStamp"] integerValue] / USEC_PER_SEC;
        //OC当中字符串带输出%需要使用%%
        LKPrint(@"#%@, %@%%, %@%%, %@%%, %@\n", @(timestamp), deviceUti, rendererUti, tilerUti, textureCount);
    }
}
