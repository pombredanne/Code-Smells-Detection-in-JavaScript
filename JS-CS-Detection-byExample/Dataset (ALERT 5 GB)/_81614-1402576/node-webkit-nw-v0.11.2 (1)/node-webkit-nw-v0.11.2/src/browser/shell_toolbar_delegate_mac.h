// Copyright (c) 2012 Intel Corp
// Copyright (c) 2012 The Chromium Authors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell co
// pies of the Software, and to permit persons to whom the Software is furnished
//  to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in al
// l copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IM
// PLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNES
// S FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WH
// ETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef CONTENT_NW_SRC_BROWSER_SHELL_TOOLBAR_DELEGATE_MAC_H_
#define CONTENT_NW_SRC_BROWSER_SHELL_TOOLBAR_DELEGATE_MAC_H_

#import <Cocoa/Cocoa.h>

#include "content/nw/src/browser/native_window.h"

namespace content {
class Shell;
}

@interface ShellToolbarDelegate : NSObject<NSToolbarDelegate> {
 @private
  content::Shell* shell_;
  NSTextField* entry_;
  NSToolbarItem* back_button_;
  NSToolbarItem* forward_button_;
  NSToolbarItem* refresh_stop_button_;
  NSToolbarItem* devtools_button_;
  NSToolbarItem* refresh_dev_button_;
}

- (id)initWithShell:(content::Shell*)shell;
- (void)buttonPressed:(id)sender;
- (void)urlEntered:(id)sender;
- (void)setEnabled:(BOOL)enabled
         forButton:(nw::NativeWindow::TOOLBAR_BUTTON)button;
- (void)setUrl:(NSString*)url;
- (void)setIsLoading:(BOOL)loading;

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar
     itemForItemIdentifier:(NSString *)itemIdentifier
 willBeInsertedIntoToolbar:(BOOL)flag;
- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar;
- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar;
@end

#endif  // CONTENT_NW_SRC_BROWSER_SHELL_TOOLBAR_DELEGATE_MAC_H_
