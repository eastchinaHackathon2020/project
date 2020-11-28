package com.mycompany.myapp.service;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import org.springframework.stereotype.Service;

@Service
public class JnaService {
    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary)
            Native.loadLibrary((Platform.isWindows() ? "msvcrt" : "c"),
                CLibrary.class);

        void printf(String format, Object... args);
    }


}
