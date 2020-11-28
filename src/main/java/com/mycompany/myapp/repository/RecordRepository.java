package com.mycompany.myapp.repository;

import com.mycompany.myapp.domain.Record;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface RecordRepository extends JpaRepository<Record, Integer> {
    List <Record> findRecordsByUserId(int userId);

    Record findRecordByFilename(String filename);

}
