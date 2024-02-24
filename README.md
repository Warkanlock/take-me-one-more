# Efficient Clustering Compression Algorithm

This repository contains the implementation and documentation for the novel data encoding compression algorithm designed to significantly reduce storage requirements for quick-capture devices like CCTV cameras and smartphones. The algorithm utilizes Huffman coding alongside Discrete Cosine Transform (DCT) techniques, focusing on minimizing data file sizes through inter-frame encoding. This approach exploits temporal redundancies between consecutive frames to achieve higher compression ratios while maintaining image quality.

## Key Features

- **Intraframe Data Encoding:** Leverages Huffman coding and DCT for effective data compression.
- **Inter-frame Encoding:** Utilizes differences between consecutive frames for higher compression ratios.
- **Dynamic Cluster Formation:** Groups similar images within a set threshold for efficient processing.
- **Memory Optimization:** Optimizes memory usage and processes differences between frames to avoid redundant information.

## Repository Structure

- `research/`: Contains the theoretical background, studies, and algorithm design principles.
- `tool/`: Implementation of the compression algorithm, including source code, binaries, and documentation.

## Implementation Overview

The implementation follows a two-step architecture:
1. **Dynamic Cluster Formation (DCF):** Continuously checks for changes in the input stream of images, grouping similar images together.
2. **Memory Optimization and Inter-frame Matching (MOIM):** Minimizes redundant processing and memory usage by efficiently matching differences between frames.

The algorithm is designed to be abstract, focusing on the core concepts rather than detailed implementations. It can be adapted to various applications requiring efficient data compression.

## Usage

Refer to the `tool/` directory for instructions on compiling and running the compression algorithm. Documentation within the `research/` directory provides a deeper understanding of the algorithm's theoretical foundation.

## Contributing

Contributions to the algorithm, whether improvements, bug fixes, or documentation, are welcome. Please refer to the contributing guidelines for more information.

## License

This project is licensed under the MIT License. Please see the `LICENSE` file for more details.
