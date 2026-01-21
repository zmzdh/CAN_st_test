# 电磁阀 PWM 设计说明

## 目标
- **不扩展 CAN 协议**：沿用现有阀门开关 mask 控制逻辑。
- **输出语义变化**：
  - `mask=1` -> 对应阀门输出 **PWM**。
  - `mask=0` -> 对应阀门输出 **无 PWM（关闭）**。
- **统一频率与占空比**：当前所有阀门保持相同的 PWM 频率与占空比。
- **预留扩展接口**：后续允许按定时器类型分组独立配置频率与占空比。

## 阀门与定时器分组（按定时器类型归并）

### GPT 组（GPT0 / GPT1 / GPT2）
- IN1  → GPT1-CH1
- IN2  → GPT1-CH2
- IN3  → GPT1-CH3
- IN4  → GPT1-CH4
- IN11 → GPT0-CH1
- IN12 → GPT0-CH2
- IN13 → GPT2-CH3
- IN14 → GPT2-CH4

### ATIM 组
- IN7  → ATIM_CH1
- IN8  → ATIM_CH2
- IN9  → ATIM_CH3
- IN10 → ATIM_CH4

### TAU 组（TAU00~07 / TAU10~17）
- IN5  → TAU04
- IN6  → TAU05
- IN15 → TAU10
- IN16 → TAU11
- IN17 → TAU12
- IN18 → TAU13
- IN19 → TAU16
- IN20 → TAU17
- IN21 → TAU14
- IN22 → TAU15
- IN23 → TAU00
- IN24 → TAU01
- IN25 → TAU02
- IN26 → TAU03
- IN27 → TAU06
- IN28 → TAU07

## 软件行为规划
- 逻辑上按 **GPT / ATIM / TAU** 三组处理 PWM。
- 当前所有组 **统一频率、统一占空比**。
- 预留接口可按组设置：
  - `SetGroupDuty(group, duty)`
  - `SetGroupFrequency(group, freq)`
- 为避免影响与 CAN 复用的引脚，**IN27/IN28 默认不参与 PWM 输出**（mask 位会被屏蔽）。

## CAN 行为说明
- 仍使用现有 `mask` 字段控制阀门开关。
- 控制语义不变，仅输出方式变更：
  - 以前：输出高/低电平
  - 现在：输出 PWM / 关闭 PWM
