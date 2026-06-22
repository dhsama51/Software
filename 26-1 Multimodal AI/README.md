# 26-1 Multimodal AI

멀티모달인공지능 과목에서 수행한 실습과 과제를 정리한 디렉토리입니다.  
이미지, 텍스트, 음성의 unimodal representation부터 image-text retrieval, image captioning, CLIP 기반 vision-language inference까지 단계적으로 실습했습니다.

## Overview

| Assignment | Topic | Main Keywords |
|---|---|---|
| Assignment 1 | Unimodal Representation | ResNet-50, GloVe, LSTM, Mel-spectrogram |
| Assignment 2 | Image-Text Retrieval | Flickr8k, Triplet Loss, Joint Embedding Space |
| Assignment 3 | Image Captioning | ResNet-50 Feature Map, LSTM Decoder, Attention, BLEU |
| Assignment 4 | CLIP Practice | Zero-shot Classification, Prompt Engineering, Image-Text Retrieval |

---

## Assignment 1: Unimodal Representation

### Objective

이미지, 텍스트, 음성이 각각 어떤 방식으로 벡터 표현으로 변환되는지 실습했습니다.

### Main Contents

| Modality | Method |
|---|---|
| Image | PIL image loading, preprocessing, ResNet-50 feature extraction |
| Text | GloVe word embedding, cosine similarity, LSTM sentence representation |
| Speech | waveform loading, Mel-spectrogram conversion |

### What I Learned

- raw image가 CNN backbone을 통해 compact feature vector로 변환되는 과정을 확인했습니다.
- GloVe embedding을 이용해 단어 간 의미적 유사도를 계산했습니다.
- LSTM을 사용해 단어 sequence를 sentence-level representation으로 변환했습니다.
- 음성 신호가 Mel-spectrogram 형태로 변환되어 모델 입력으로 사용될 수 있음을 확인했습니다.

---

## Assignment 2: Image-Text Retrieval with Triplet Loss

### Objective

이미지와 텍스트를 같은 embedding space에 매핑한 뒤, 주어진 문장과 가장 관련 있는 이미지를 검색하는 image-text retrieval 모델을 구현했습니다.

### Model Structure

| Component | Description |
|---|---|
| Image Encoder | Pre-extracted image feature를 projection layer로 embedding space에 매핑 |
| Text Encoder | GloVe embedding + LSTM으로 caption representation 생성 |
| Projection | FC projection과 MLP projection 비교 |
| Loss | Hard negative 기반 triplet loss |
| Evaluation | Image-to-Text / Text-to-Image Recall@K |

### Experiments

| Setting | I→T R@1 | I→T R@5 | I→T R@10 | T→I R@1 | T→I R@5 | T→I R@10 |
|---|---:|---:|---:|---:|---:|---:|
| Random Initialization | 0.1 | 0.5 | 1.3 | 0.1 | 0.6 | 1.2 |
| FC Projection | 2.8 | 11.6 | 19.3 | 0.4 | 2.0 | 3.2 |
| MLP Projection | 5.8 | 18.4 | 28.3 | 6.3 | 19.0 | 26.7 |

### What I Learned

- 이미지와 텍스트를 같은 joint embedding space에 정렬하는 기본 구조를 이해했습니다.
- Triplet loss와 hard negative mining이 retrieval 성능에 미치는 영향을 확인했습니다.
- 단순 FC projection보다 MLP projection이 더 나은 retrieval 성능을 보였습니다.

---

## Assignment 3: Image Captioning

### Objective

이미지를 입력받아 자연어 caption을 생성하는 모델을 구현하고, attention을 적용한 모델과 비교했습니다.

### Model Structure

| Model | Description |
|---|---|
| Captioning Base | ResNet-50 image feature를 평균 pooling한 뒤 LSTM decoder로 caption 생성 |
| Captioning + Attention | 7×7 spatial feature map에 attention을 적용하여 decoding step마다 다른 image region 참조 |

### Main Components

- Flickr8k dataset parsing
- train / validation / test split
- ResNet-50 feature map extraction: 2048×7×7
- GloVe 기반 vocabulary 구성
- LSTM decoder training
- BLEU score 기반 caption 평가
- Base model과 Attention model의 training loss 비교

### What I Learned

- Image captioning은 vision encoder와 language decoder가 결합된 대표적인 multimodal task임을 이해했습니다.
- Attention mechanism을 통해 caption 생성 과정에서 이미지의 spatial feature를 동적으로 참조할 수 있음을 확인했습니다.
- BLEU score를 사용해 generated caption과 reference caption의 유사도를 평가했습니다.

---

## Assignment 4: CLIP Practice

### Objective

CLIP 모델을 사용하여 image-text embedding, zero-shot image classification, image-text retrieval을 실습했습니다.

### Main Contents

| Task | Description |
|---|---|
| Image/Text Embedding | CLIP image encoder와 text encoder를 사용해 같은 차원의 embedding 생성 |
| Similarity Matrix | cosine similarity 기반 image-text matching 확인 |
| Zero-shot Classification | CIFAR-10 class name prompt만으로 이미지 분류 수행 |
| Prompt Engineering | class name을 자연어 prompt로 변환하여 성능 확인 |
| Image-Text Retrieval | 텍스트 query와 이미지 embedding 간 similarity로 검색 수행 |

### Results

| Task | Result |
|---|---:|
| CIFAR-10 Zero-shot Accuracy | 88.8% |
| Text → Image Retrieval R@1 | 52.9% |
| Text → Image Retrieval R@5 | 81.9% |
| Text → Image Retrieval R@10 | 88.6% |
| Image → Text Retrieval R@1 | 55.0% |
| Image → Text Retrieval R@5 | 82.8% |
| Image → Text Retrieval R@10 | 90.7% |

### What I Learned

- CLIP은 이미지와 텍스트를 같은 embedding space에 정렬하여 zero-shot classification과 retrieval에 활용할 수 있음을 확인했습니다.
- 단순 class name보다 자연어 prompt를 사용하는 방식이 vision-language model 활용에서 중요함을 배웠습니다.
- Multimodal representation learning이 image classification, retrieval, visual grounding 등 다양한 task로 확장될 수 있음을 이해했습니다.

## Files

```text
.
├── Assignment1_Unimodal/
│   └── MAI_asgn1_20212052_이동훈.ipynb
├── Assignment2_Image_Text_Retrieval/
│   ├── MAI_asgn2_20212052_이동훈.ipynb
│   └── report.pdf
├── Assignment3_Image_Captioning/
│   ├── captioning_training.ipynb
│   └── report.pdf
└── Assignment4_CLIP_Practice/
    ├── clip_practice.ipynb
    └── report.pdf
```

## Keywords

`Multimodal AI` `Computer Vision` `Vision-Language Models` `Image-Text Retrieval` `Image Captioning` `CLIP` `ResNet-50` `GloVe` `LSTM` `Attention`
