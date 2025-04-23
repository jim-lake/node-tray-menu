export type MenuItemParams = {
  click?: () => void;
  type: 'normal' | 'separator';
  label?: string;
  enabled?: boolean;
};
export declare class MenuItem {
  _index: number;
  _label: string;
  _click: () => void | undefined;
  constructor(params: MenuItemParams);
  get label(): string;
  set label(new_label: string);
  destroy(): void;
}
export declare class Menu {
  _index: number;
  items: MenuItem[];
  constructor();
  append(menuItem: MenuItem): void;
  insert(pos: number, menuItem: MenuItem): void;
  remove(pos: number): void;
  destroy(): void;
}
export declare class Tray {
  _index: number;
  constructor(image: any, menu: Menu);
  destroy(): void;
}
declare const _default: {
  MenuItem: typeof MenuItem;
  Menu: typeof Menu;
  Tray: typeof Tray;
};
export default _default;
