//
//  iOSLogger.m
//  Doom
//
//  Created by Alasdair Morrison on 24/08/2013.
//
//

#import "iOSLogger.h"

@implementation iOSLogger

@end

extern void alang_log_one_func_to_db(int change_limit);

void start_logging_thread() {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        while (true) {
            //NSLog(@"Background task");
            alang_log_one_func_to_db(10000);
        }
    });
}

const char* get_dbpath_path() {
    
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentsDirectory = [paths lastObject];
    NSString* databasePath = [documentsDirectory stringByAppendingPathComponent:@"alang_ios.sqlite"];
    return [databasePath UTF8String];
}