#import "TrayMenu.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface MyMenuItem : NSMenuItem

- (id)initWithIndex:(int)index
              title:(NSString *)title
        hasCallback:(BOOL)hasCallback;

@end

static int g_count = 1;
static ClickCallback g_clickCallback = NULL;
static NSMutableDictionary *g_itemMap = nil;

void setCallback(const ClickCallback func) {
  g_itemMap = [NSMutableDictionary new];
  g_clickCallback = func;
}
int MenuItemCreate(const std::string &input, const bool has_callback) {
  const auto index = g_count++;
  NSString *title = [NSString stringWithUTF8String:input.c_str()];
  id mi = [[MyMenuItem alloc] initWithIndex:index
                                      title:title
                                hasCallback:has_callback];
  [g_itemMap setObject:mi forKey:[NSNumber numberWithInteger:index]];
  return index;
}
int MenuItemCreateSeparator() {
  const auto index = g_count++;
  [g_itemMap setObject:[NSMenuItem separatorItem]
                forKey:[NSNumber numberWithInteger:index]];
  return index;
}
void MenuItemUpdate(const int index, const std::string &input) {
  MyMenuItem *mi = [g_itemMap objectForKey:[NSNumber numberWithInteger:index]];
  NSString *title = [NSString stringWithUTF8String:input.c_str()];
  [mi setTitle:title];
}
void ItemDestroy(const int index) {
  [g_itemMap removeObjectForKey:[NSNumber numberWithInteger:index]];
}
int MenuCreate() {
  const auto index = g_count++;
  const auto menu = [[NSMenu alloc] initWithTitle:@""];
  [menu setAutoenablesItems:TRUE];
  [g_itemMap setObject:menu forKey:[NSNumber numberWithInteger:index]];
  return index;
}
void MenuInsertItem(const int menu_index, const int menu_item_index,
                    const int index) {
  NSMenu *menu =
      [g_itemMap objectForKey:[NSNumber numberWithInteger:menu_index]];
  NSMenuItem *item =
      [g_itemMap objectForKey:[NSNumber numberWithInteger:menu_item_index]];
  if (menu != nil && item != nil) {
    [menu insertItem:item atIndex:index];
  } else {
    NSLog(@"MenuInsertItem: menu insert item failed: %d, %d, %d", menu_index,
          menu_item_index, index);
  }
}
void MenuRemoveItem(const int menu_index, const int index) {
  NSMenu *menu =
      [g_itemMap objectForKey:[NSNumber numberWithInteger:menu_index]];
  [menu removeItemAtIndex:index];
}
int TrayCreate(const std::string &path, const int menu_index) {
  const auto index = g_count++;
  NSString *file = [NSString stringWithUTF8String:path.c_str()];
  NSImage *image = [[NSImage alloc] initWithContentsOfFile:file];
  NSMenu *menu =
      [g_itemMap objectForKey:[NSNumber numberWithInteger:menu_index]];
  if (menu != nil) {
    //    dispatch_async(dispatch_get_main_queue(), ^(void){
    NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
    // NSVariableStatusItemLength
    // NSSquareStatusItemLength
    NSStatusItem *item =
        [statusBar statusItemWithLength:NSVariableStatusItemLength];
    [g_itemMap setObject:item forKey:[NSNumber numberWithInteger:index]];
    [item setMenu:menu];
    if (image != nil) {
      item.button.image = image;
    } else {
      NSLog(@"TrayCreate: bad image: %s", path.c_str());
    }
    //  });
  } else {
    NSLog(@"TrayCreate: failed to set menu: %d", menu_index);
  }
  return index;
}
void TrayDestroy(const int index) {
  NSStatusItem *item =
      [g_itemMap objectForKey:[NSNumber numberWithInteger:index]];
  if (item != nil) {
    NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
    [statusBar removeStatusItem:item];
    ItemDestroy(index);
  } else {
    NSLog(@"TrayDestroy: bad index: %d", index);
  }
}
@implementation MyMenuItem {
  int _index;
}
- (id)initWithIndex:(int)index
              title:(NSString *)title
        hasCallback:(BOOL)hasCallback {
  const SEL action = hasCallback ? @selector(myClick:) : NULL;
  self = [super initWithTitle:title action:action keyEquivalent:@""];
  if (self) {
    self->_index = index;
    [self setTarget:self];
  }
  return self;
}
- (void)myClick:(NSMenuItem *)menuItem {
  NSLog(@"click: %d", self->_index);
  if (g_clickCallback != NULL) {
    g_clickCallback(self->_index);
  }
}
@end
