declare const addon: any;
declare const g_itemMap: Map<number, MenuItem>;
declare function _callback(index: number): void;
type MenuItemParams = {
  click?: () => void;
  type: 'normal' | 'separator';
  label?: string;
  enabled?: boolean;
};
declare class MenuItem {
  _index: number;
  _label: string;
  _click: () => void | undefined;
  constructor(params: MenuItemParams);
  get label(): string;
  set label(new_label: string);
  destroy(): void;
}
declare class Menu {
  _index: number;
  items: MenuItem[];
  constructor();
  append(menuItem: MenuItem): void;
  insert(pos: number, menuItem: MenuItem): void;
  remove(pos: number): void;
  destroy(): void;
}
declare class Tray {
  _index: number;
  constructor(image: any, menu: Menu);
  destroy(): void;
}
