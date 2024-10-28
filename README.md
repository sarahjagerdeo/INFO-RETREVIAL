# Search Engine Project

### Overview
This project is a C-based search engine designed to scan and analyze text documents to identify and rank their relevance to user queries. Using hash maps and efficient data structures, this search engine organizes and retrieves documents based on relevance, allowing for quick and accurate results.

### Key Features
- **Hash Maps for Data Retrieval**: Utilizes hash maps to efficiently store and retrieve data.
- **Hashing for Fast Lookup**: Employs hashing to manage large datasets and provide rapid search capabilities.
- **TF-IDF Scoring**: Implements the Term Frequency-Inverse Document Frequency (TF-IDF) method to assess the relevance of documents based on the frequency of terms.

### Technology Stack
- **Language**: C
- **Data Structures**: Hash maps
- **Relevance Algorithm**: TF-IDF for ranking document relevance

### Usage
1. **Compile** the project using a C compiler (e.g., `gcc`).
   ```bash
   gcc -o search_engine search_engine.c
