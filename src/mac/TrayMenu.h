#pragma once

#include <string>

typedef void (*ClickCallback)(const long index);

extern void setCallback(const ClickCallback func);
extern int MenuItemCreate(const std::string &input, const bool has_callback);
extern int MenuItemCreateSeparator();
extern void MenuItemUpdate(const int index, const std::string &input);
extern void ItemDestroy(const int index);

extern int MenuCreate();
extern void MenuInsertItem(const int menu_index, const int menu_item_index,
                           const int index);
extern void MenuRemoveItem(const int menu_index, const int index);

int TrayCreate(const std::string &path, const int menu_index);
extern void TrayDestroy(const int index);
