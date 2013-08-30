//
//  iOSLogger.m
//  Doom
//
//  Created by Alasdair Morrison on 24/08/2013.
//
//

#import "iOSLogger.h"
#import "EAGLView.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
//#include "log_functions.h"


@implementation iOSLogger

@end

extern EAGLView *eaglview;
GLubyte *alang_screenshot_buffer=0;
NSInteger alang_screenshot_buffer_length=0;
bool alang_taken_screenshot=false;
GLint alang_screenshot_width=0;

GLint alang_screenshot_height=0;
long long alang_screenshot_time_logged;
//extern sqlite3 *ali__log__db;

extern void execute_query(const char* query);

void getImage (bool isMainLoop) {
    
    if (isMainLoop && !alang_taken_screenshot) {
        alang_screenshot_width=eaglview->backingWidth;
        
        alang_screenshot_height=eaglview->backingHeight;
        
        //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        
        //glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        
        
        NSLog(@" took screen shot %d %d",alang_screenshot_width,alang_screenshot_height);
        
        alang_screenshot_buffer_length = alang_screenshot_width * alang_screenshot_height * 4;
        
        // allocate array and read pixels into it.
        alang_screenshot_buffer = (GLubyte *) malloc(alang_screenshot_buffer_length);
        glReadPixels(0, 0, alang_screenshot_width, alang_screenshot_height, GL_RGBA, GL_UNSIGNED_BYTE, alang_screenshot_buffer);
        alang_taken_screenshot=true;
        alang_screenshot_time_logged = clock();
            return;
    }
    
    if (!isMainLoop && alang_taken_screenshot == true) {
        // gl renders "upside down" so swap top to bottom into new array.
        // there's gotta be a better way, but this works.
        GLubyte *buffer2 = (GLubyte *) malloc(alang_screenshot_buffer_length);
        for(int y = 0; y < alang_screenshot_height; y++)
        {
            for(int x = 0; x < alang_screenshot_width * 4; x++)
            {
                buffer2[((alang_screenshot_height - 1) - y) * alang_screenshot_width * 4 + x] = alang_screenshot_buffer[y * 4 * alang_screenshot_width + x];
            }
        }
        
        // make data provider with data.
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer2, alang_screenshot_buffer_length, NULL);
        
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * alang_screenshot_width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(alang_screenshot_width, alang_screenshot_height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage *myImage = [UIImage imageWithCGImage:imageRef];
        
            
            NSString *image_filename = [NSString stringWithFormat:@"/Alang_Screenshots/%lld_screen.jpg",alang_screenshot_time_logged];//@"_screen.jpg";
        
        // Create path.
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *filePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:image_filename];
        
        // Save image.
        [UIImageJPEGRepresentation(myImage, 0.001f) writeToFile:filePath atomically:YES];
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpaceRef);
        free(alang_screenshot_buffer);
        free(buffer2);
        
        char * sErrMsg = 0;
        NSString *query = [NSString stringWithFormat:@"INSERT INTO alang_screenshots VALUES (%lld,'/Alang_Screenshots/%lld_screen.jpg')",alang_screenshot_time_logged,alang_screenshot_time_logged];
        //sqlite3_exec(ali__log__db, [query UTF8String], NULL, NULL, &sErrMsg);
        execute_query([query UTF8String]);
            alang_taken_screenshot=false;
            NSLog(@" written screen shot %d %d",alang_screenshot_width,alang_screenshot_height);
    }
    
}

extern void alang_log_one_func_to_db(int change_limit);

void start_logging_thread() {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        while (true) {
            //NSLog(@"Background task");
            alang_log_one_func_to_db(10000);
            getImage(false);
        }
    });
}

const char* get_dbpath_path() {
    
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentsDirectory = [paths lastObject];
    NSString* databasePath = [documentsDirectory stringByAppendingPathComponent:@"alang_ios.sqlite"];
    //delete the file
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtPath:databasePath error:NULL];
    NSString* imageFolder = [documentsDirectory stringByAppendingPathComponent:@"/Alang_Screenshots"];
    BOOL success = [[NSFileManager defaultManager] removeItemAtPath:imageFolder error:nil];
    [[NSFileManager defaultManager] createDirectoryAtPath:imageFolder withIntermediateDirectories:NO attributes:nil error:nil];
    
    return [databasePath UTF8String];
}