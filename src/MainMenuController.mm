////
////  MainMenuViewController.m
////  MainMenu
////
////  Created by Nick Hardeman on 3/17/14.
////
////
//
//#import "MainMenuController.h"
//#include "testApp.h"
//
//@interface MainMenuController ()
//    
//@end
//
//@implementation MainMenuController
//
//- (id)init {
//    
//    if((self = [super init])) {
//        
//        NSMenu* bar = [[NSApplication sharedApplication] mainMenu];
//        [bar removeAllItems];
//        
//        NSMenuItem* appMenuItem = [bar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
//        NSMenu* appMenu = [[NSMenu alloc] init];
//        [appMenuItem setSubmenu:appMenu];
//        
//        
//        [appMenu addItem:[NSMenuItem separatorItem]];
//        // Audio Settings ----
////        appMenuItem = [[NSMenuItem alloc] initWithTitle:@"Add Circle"
////                           action:@selector(onAddCircleSelected:)
////                    keyEquivalent:@"c"];
////        [appMenuItem setKeyEquivalentModifierMask:NSCommandKeyMask];
////        [appMenuItem setEnabled:YES];
////        [appMenuItem setTarget:self];
////        [appMenu addItem:appMenuItem];
////        [appMenuItem release];
//        // end audio settings ---- //
//        
//        
//        // Addr Mode ---- //
//        [appMenu addItem:[NSMenuItem separatorItem]];
//        appMenuItem = [[NSMenuItem alloc] initWithTitle:@"Toggle Address Mode"
//                                                 action:@selector(toggleAddressMode:)
//                                          keyEquivalent:@"f"];
//        [appMenuItem setKeyEquivalentModifierMask:NSCommandKeyMask];
//        [appMenuItem setEnabled:YES];
//        [appMenuItem setTarget:self];
//        [appMenu addItem:appMenuItem];
//        [appMenuItem release];
//        // end Addr Mode ---- //
//        
//        
//        // Quit App ---- //
//        [appMenu addItem:[NSMenuItem separatorItem]];
//        appMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit"
//                                                 action:@selector(onQuitAppSelected:)
//                                          keyEquivalent:@"q"];
//        [appMenuItem setKeyEquivalentModifierMask:NSCommandKeyMask];
//        [appMenuItem setEnabled:YES];
//        [appMenuItem setTarget:self];
//        [appMenu addItem:appMenuItem];
//        [appMenuItem release];
//        // end Quit App ---- //
//    }
//    
//    return self;
//}
//
//- (void) dealloc {
//    [super dealloc];
//}
//
////- (void)toggleFullScreen:(id)sender {
////    testApp* appPtr = (testApp*)ofGetAppPtr();
////    appPtr->onFullScreenSelected();
////}
//
//- (void)toggleAddressMode:(id)sender {
//    testApp* appPtr = (testApp*)ofGetAppPtr();
//    appPtr->toggleAddressMode();
//}
//
//- (void)onQuitAppSelected:(id)sender {
//    ofExit();
//}
//
//@end
