# Keil 编译错误修复报告

日期：2026-09-05

工程：`D:\CubelMX\24Lvgl_Dashboard\MDK-ARM\12LCD_Display.uvprojx`

## 结论

已修复导致本次构建失败的编译后命令路径错误。使用本机 Keil 完整重编译，最终结果为 `0 Error(s), 49 Warning(s)`。本次验证覆盖编译、链接、HEX 生成和编译后工具运行，未进行开发板烧录或屏幕、触摸等硬件验证。

## 原因和位置

原日志显示源码编译、链接及 HEX 生成已完成，随后报错：

```text
After Build - User command #1: D:\uint3code\24Lvgl_Dashboard\Keil5_disp_size_bar.exe
*** Error: CreateProcess failed, Command: 'D:\uint3code\24Lvgl_Dashboard\Keil5_disp_size_bar.exe'
"12LCD_Display\12LCD_Display.axf" - 1 Error(s), 49 Warning(s).
```

`MDK-ARM/12LCD_Display.uvprojx` 第 82 至 91 行的 `AfterMake` 配置保留了旧目录的绝对路径。工程现在位于 `D:\CubelMX\24Lvgl_Dashboard`，容量统计工具也在当前工程根目录，旧路径不存在，因此 Keil 无法启动该程序，并把整次构建标记为失败。

另有一条未填写程序名的编译后命令被勾选，本次一并关闭；它不是日志中已确认的直接报错来源。

## 已执行的修改

1. 将原工程配置和失败日志备份到 `repair-backup-20260905-220021/`。
2. 修改 `MDK-ARM/12LCD_Display.uvprojx` 第 85 行，将第一条命令改为 `"..\Keil5_disp_size_bar.exe"`。相对路径以 Keil 工程所在的 `MDK-ARM` 目录为基准，指向上一级目录中的工具；保留该工具的容量统计功能。
3. 将第 84 行 `RunUserProg2` 从 `1` 改为 `0`，关闭空的第二条命令。
4. 解析修改前后的工程 XML，确认仅以上两项配置发生变化。
5. 执行完整重编译，并核对最终日志和新生成文件的时间。

## 验证结果

使用 `D:\UV4\UV4.exe`，编译器为 ARMCC 5.06 update 7。完整重编译命令如下，可在 PowerShell 中执行：

```powershell
$build = Start-Process -FilePath 'D:\UV4\UV4.exe' -ArgumentList '-r', '"D:\CubelMX\24Lvgl_Dashboard\MDK-ARM\12LCD_Display.uvprojx"', '-j0', '-o', '"D:\CubelMX\24Lvgl_Dashboard\MDK-ARM\12LCD_Display\codex_rebuild.log"' -WorkingDirectory 'D:\CubelMX\24Lvgl_Dashboard\MDK-ARM' -WindowStyle Hidden -PassThru
$build.WaitForExit()
Get-Content 'D:\CubelMX\24Lvgl_Dashboard\MDK-ARM\12LCD_Display\codex_rebuild.log' -Tail 25
```

实际构建结果：

```text
Program Size: Code=168206 RO-data=307022 RW-data=684 ZI-data=62588
After Build - User command #1: "..\Keil5_disp_size_bar.exe"
[Keil5_disp_size_bar V0.4]: map file read end
"12LCD_Display\12LCD_Display.axf" - 0 Error(s), 49 Warning(s).
Build Time Elapsed: 00:00:09
```

Keil 进程返回码为 `1`，对应有警告的构建；最终日志确认错误数为零，不能把这个返回码直接理解为源码编译失败。

| 文件 | 本次生成时间 | 大小 |
| --- | --- | --- |
| `MDK-ARM/12LCD_Display/12LCD_Display.axf` | 2026-09-05 22:00:30 | 5,086,152 字节 |
| `MDK-ARM/12LCD_Display/12LCD_Display.hex` | 2026-09-05 22:00:31 | 1,337,329 字节 |
| `MDK-ARM/12LCD_Display/codex_rebuild.log` | 2026-09-05 22:00:32 | 16,029 字节 |

AXF 含调试信息，HEX 为文本编码文件，其文件大小不等于实际 Flash 占用。链接 map 中 Total ROM Size 为 475,432 字节，约 464.29 KiB，占 512 KiB 的约 90.68%；Total RW Size 为 63,272 字节，占 64 KiB RAM 的约 96.55%，剩余 2,264 字节未分配给静态段。该余量不代表运行时堆或栈的可用余量，运行时水位尚未测量。

## 在 Keil 中复查的步骤

1. 当前 Keil 已打开该工程。关闭并重新打开上述 `.uvprojx`，确保加载磁盘上修正后的配置；若提示保存旧工程配置，避免用旧配置覆盖本次修改。
2. 打开 `Options for Target -> User -> After Build/Rebuild`。
3. 确认第一条命令已启用，内容为 `"..\Keil5_disp_size_bar.exe"`；第二条空命令未勾选。
4. 执行 `Rebuild all target files`，在 Build Output 末尾核对 `0 Error(s)`，并确认容量统计信息正常显示。
5. 后续烧录时使用本次生成的固件，在开发板上检查仪表盘显示、颜色、触摸和时间更新。

## 剩余警告说明

49 条警告仍存在，与修复前数量一致，主要包括 GPIO/LVGL 枚举与整数混用、未使用局部变量、不可达语句、跳转绕过初始化、整数符号转换，以及显示移植模板的分辨率提示。分辨率代码已使用 `LCD_W` 和 `LCD_H`，但保留了模板提示文字。

当前工程的 LVGL 和 SquareLine 导出均为 8.3.11，`ui.c` 要求的 `LV_COLOR_16_SWAP=1` 与 `lv_conf.h` 一致。本次检查没有发现旧版本案例中的图片 API 或颜色交换配置冲突，因此仅修改本次失败直接涉及的构建配置。剩余警告未逐项修复，不能据此宣称所有运行时问题已排除。
