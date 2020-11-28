package com.mycompany.myapp.service.impl;

import com.mycompany.myapp.service.IUploadService;
//import com.mycompany.myapp.vo.Chunk;
import lombok.extern.slf4j.Slf4j;
import org.h2.mvstore.Chunk;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.*;

/**
 * @author vi
 * @since 2019/5/9 8:37 PM
 */
@Service
@Slf4j
@Async
public class UploadServiceImpl implements IUploadService {

    @Autowired

    private static final String mergeFolder = "/Users/yangwei/resource/data/merge";


    @Override
    public Map<String, Object> checkChunkExits(Chunk chunk) {
        return null;
    }

    @Override
    public Integer saveChunk(Integer chunkNumber, String identifier) {
        return null;
    }

    @Override
    public void mergeFile(String fileName, String chunkFolder) {
        try {
            if (!Files.isWritable(Paths.get(mergeFolder))) {
                Files.createDirectories(Paths.get(mergeFolder));
            }
            String target = mergeFolder + File.separator + fileName;
            Files.createFile(Paths.get(target));
            Files.list(Paths.get(chunkFolder))
                    .filter(path -> path.getFileName().toString().contains("-"))
                    .sorted((o1, o2) -> {
                        String p1 = o1.getFileName().toString();
                        String p2 = o2.getFileName().toString();
                        int i1 = p1.lastIndexOf("-");
                        int i2 = p2.lastIndexOf("-");
                        return Integer.valueOf(p2.substring(i2)).compareTo(Integer.valueOf(p1.substring(i1)));
                    })
                    .forEach(path -> {
                        try {
                            //以追加的形式写入文件
                            Files.write(Paths.get(target), Files.readAllBytes(path), StandardOpenOption.APPEND);
                            //合并后删除该块
                            Files.delete(path);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    });
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
