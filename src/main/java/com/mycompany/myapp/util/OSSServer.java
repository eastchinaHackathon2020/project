package com.mycompany.myapp.util;

import com.aliyun.oss.OSSClient;
import com.aliyun.oss.model.ObjectMetadata;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

public class OSSServer {
    private static final String accessKeyId = "LTAI4G8wqGH4HdWASLz5Ziic";
    private static final String accessKeySecret = "11bpMAvbBroFCjZ80xKOHktmWo6iJH";
    private static final String endpoint = "oss-cn-beijing.aliyuncs.com";
    private static final String bucketName = "gitvisual";

    public static void main(String[] args) {
        String objectName = "C:\\Users\\admin\\Desktop\\test.png";
        // 创建OSSClient实例。
        OSSClient ossClient = new OSSClient(endpoint, accessKeyId, accessKeySecret);
        // 上传文件
        ObjectMetadata meta = new ObjectMetadata();
        meta.setContentType("image/jpg");
        // 上传文件流。
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(objectName);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        ossClient.putObject(bucketName, "test.png", inputStream,meta);
        // 关闭OSSClient。
        ossClient.shutdown();
    }
}
