#import <Preferences/PSListController.h>
#import <Preferences/PSSpecifier.h>
#import "NSTask.h"

#define THEME_COLOR [UIColor colorWithRed:0.145 green:0.122 blue:0.196 alpha:1.0];

@interface DCKAppearanceSettings : NSObject

@end

@interface DCKPrefsListController : PSListController {
    UITableView * _table;
}

@property (nonatomic, retain) UIBarButtonItem *respringButton;
@property (nonatomic, retain) UIView *headerView;
@property (nonatomic, retain) UIImageView *headerImageView;
@property (nonatomic, retain) UILabel *titleLabel;
@property (nonatomic, retain) UIImageView *iconView;
- (void)respring:(id)sender;

@end