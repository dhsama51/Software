# 26-1 AI Hardware

인공지능하드웨어 과목에서 수행한 과제를 정리한 디렉토리입니다.  
딥러닝 연산이 실제 하드웨어에서 어떻게 계산되고, 메모리 접근과 dataflow에 따라 성능이 어떻게 달라지는지 이해하는 것을 목표로 했습니다.

## Overview

| Assignment | Topic | Main Keywords |
|---|---|---|
| Practice 1 | Matrix Multiplication with Tiling | Convolution Lowering, Matrix Tiling, `np.matmul` |
| Practice 2 | AI Accelerator | Dataflow Analysis, Roofline Model, Weight Stationary Systolic Array |

---

## Practice 1: Matrix Multiplication with Tiling

### Objective

Convolution 연산을 matrix multiplication 형태로 변환하고, tiling을 적용했을 때 실행 시간이 어떻게 달라지는지 분석했습니다.

### Main Contents

| Topic | Description |
|---|---|
| Convolution Lowering | convolution input patch와 kernel을 matrix 형태로 변환 |
| Matrix Tiling | 전체 행렬곱을 T×T tile 단위로 나누어 계산 |
| Manual MatMul | `ii`, `jj`, `kk` 반복문을 이용한 tile 내부 직접 계산 |
| `np.matmul` | NumPy 행렬곱을 이용한 tile 단위 계산 |
| Performance Comparison | original / tiling / tiling + `np.matmul` 실행 시간 비교 |

### Key Idea

Convolution lowering은 convolution을 행렬곱으로 바꾸기 위해 입력 이미지의 receptive field를 한 행으로 펼치고, kernel도 2D matrix로 변환하는 방법입니다. 이 과정에서 같은 input image 값이 여러 receptive field에 포함될 수 있으므로 중복 저장이 발생하지만, 이후 행렬곱 최적화를 적용하기 쉬워집니다.

Matrix tiling은 큰 행렬곱을 작은 tile 단위로 나누어 계산하여 cache locality와 연산 효율을 높이는 방식입니다.

### Results Summary

- 기본 convolution 구현보다 convolution lowering과 matrix tiling을 적용했을 때 더 빠른 실행 시간을 보였습니다.
- 직접 반복문으로 tile 내부 곱을 계산하는 것보다 `np.matmul`을 사용하는 방식이 훨씬 빨랐습니다.
- 실험에서는 T=64와 `np.matmul`을 함께 사용했을 때 가장 빠른 실행 시간이 관찰되었습니다.

### What I Learned

- CNN의 convolution 연산이 GEMM 형태로 변환될 수 있음을 이해했습니다.
- 같은 연산이라도 memory access pattern과 library optimization에 따라 실행 시간이 크게 달라질 수 있음을 확인했습니다.
- AI accelerator에서 연산량뿐 아니라 데이터 이동과 메모리 접근 효율이 중요하다는 점을 배웠습니다.

---

## Practice 2: AI Accelerator

### Objective

GEMM workload를 대상으로 dataflow별 DRAM 접근량을 계산하고, weight-stationary systolic array simulator를 Python으로 구현했습니다.

### Problem 1: Dataflow Analysis

분석 대상 GEMM은 다음과 같습니다.

```text
Q = X · W_Q
X   : [512 × 512], INT8
W_Q : [512 × 512], INT8
Q   : [512 × 512], INT8
```

하드웨어 조건은 다음과 같습니다.

| Item | Value |
|---|---:|
| PE Array | 16 × 16 |
| Global Buffer | 512 KB |
| DRAM Bandwidth | 256 GB/s |
| Peak Compute | 2 TOPS INT8 |
| Sequence Length | 512 |
| d_model | 512 |
| Num Heads | 8 |
| d_k | 64 |

### Dataflow Comparison

| Dataflow | Total DRAM Access | Arithmetic Intensity | Region |
|---|---:|---:|---|
| Weight Stationary | 128.50 MB | 1.9922 FLOP/byte | Memory-Bound |
| Output Stationary | 257.25 MB | 0.9951 FLOP/byte | Memory-Bound |
| Input Stationary | 128.50 MB | 1.9922 FLOP/byte | Memory-Bound |

### Selected Dataflow

최적 dataflow로 **Weight Stationary**를 선택했습니다.

주요 이유는 다음과 같습니다.

1. **Global Buffer 제약**  
   W_Q는 256 KB로 512 KB Global Buffer 안에 올릴 수 있지만, Output Stationary 방식에서 INT32 partial sum 전체를 저장하려면 1 MB가 필요해 제약을 초과합니다.

2. **Inference 시 Weight Reuse**  
   추론에서는 weight가 고정되어 반복적으로 사용되므로, weight를 on-chip에 유지하는 방식이 DRAM 접근을 줄이는 데 유리합니다.

3. **INT32 Partial Sum 문제**  
   partial sum은 INT32로 누산되어 메모리 사용량이 커지므로 Output Stationary 방식은 buffer 측면에서 불리합니다.

4. **Systolic Array 구현 친화성**  
   각 PE에 weight를 고정하고 activation을 전달하는 구조는 2D systolic array로 구현하기 쉽습니다.

---

## Problem 2: Weight-Stationary Systolic Array Simulator

### Architecture

- N×N Processing Element array
- 각 PE는 하나의 stationary weight를 보유
- Activation은 왼쪽에서 오른쪽으로 pass-through
- 출력은 `C = A @ W.T`
- 총 cycle 수는 `K + 2 × (N - 1)`로 계산

### Implemented Components

| Component | Description |
|---|---|
| `PE.compute()` | `psum += weight × activation` 수행 후 activation 전달 |
| `load_weights(W)` | `C = A @ W.T`가 성립하도록 PE에 weight 배치 |
| `_skew_input(A)` | systolic skew를 반영한 cycle별 activation schedule 생성 |
| `run(A)` | cycle 단위로 PE array를 실행하고 output matrix 반환 |
| `readout()` | PE의 partial sum을 output matrix로 변환 |

### Correctness Check

구현한 systolic array의 출력이 NumPy reference 결과와 일치함을 확인했습니다.

```text
C_hw == A @ W.T  PASSED
total_cycles == K + 2·(N-1)  PASSED
```

### PE Utilization Analysis

| N | Total Cycles | PE Utilization | Skew Latency |
|---:|---:|---:|---:|
| 4 | 10 | 40.00% | 6 |
| 8 | 22 | 36.36% | 14 |
| 16 | 46 | 34.78% | 30 |
| 32 | 94 | 34.04% | 62 |

PE utilization은 다음과 같이 계산했습니다.

```text
PE Utilization = K / (K + 2(N - 1)) × 100
```

K=N인 경우 N이 커질수록 setup/drain skew latency의 영향 때문에 활용률이 약 1/3 수준으로 수렴하는 경향을 보였습니다.

## What I Learned

- 딥러닝 연산의 병목이 단순 연산량이 아니라 memory bandwidth와 data movement에서 발생할 수 있음을 이해했습니다.
- Weight Stationary, Output Stationary, Input Stationary dataflow의 차이를 DRAM 접근량 관점에서 비교했습니다.
- Systolic array의 skew scheduling, setup latency, drain latency가 PE utilization에 영향을 준다는 것을 확인했습니다.
- AI 모델을 효율적으로 실행하기 위해서는 알고리즘뿐 아니라 하드웨어 구조와 데이터 흐름을 함께 고려해야 함을 배웠습니다.

## Files

```text
.
├── 20212052-이동훈-p1.pdf
└── 20212052 이동훈 인공지능하드웨어 과제2.ipynb
```

## Keywords

`AI Hardware` `AI Accelerator` `Convolution Lowering` `Matrix Tiling` `GEMM` `Dataflow Analysis` `Roofline Model` `Systolic Array` `Weight Stationary`
