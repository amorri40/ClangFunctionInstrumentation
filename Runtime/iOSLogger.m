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

const char* get_dbpath_path() {
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentsDirectory = [paths lastObject];
    NSString* databasePath = [documentsDirectory stringByAppendingPathComponent:@"alang_ios.sqlite"];
    return [databasePath UTF8String];
}