package com.ice3037.backend.farmer;

import com.ice3037.backend.farmer.domain.Farmer;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface FarmerRepository extends JpaRepository<Farmer, Long> {

    Farmer findByName(String name);

}
