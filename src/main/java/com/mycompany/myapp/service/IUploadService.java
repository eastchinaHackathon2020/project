package com.mycompany.myapp.service;

//import com.mycompany.myapp.vo.Chunk;
import org.h2.mvstore.Chunk;

import java.util.Map;

public interface IUploadService {

    Map<String, Object> checkChunkExits(Chunk chunk);

    Integer saveChunk(Integer chunkNumber, String identifier);

    void mergeFile(String fileName, String chunkFolder);
}
