package com.ice3037.backend.farmer;


import com.ice3037.backend.farmer.domain.Farmer;
import com.ice3037.backend.farmer.service.AreaFarmerService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@RequiredArgsConstructor
@Transactional
@Service
public class FarmerService {

    private final FarmerRepository farmerRepository;
    private final AreaFarmerService areaFarmerService;

    public void createFarmer(final String name) {
        farmerRepository.save(Farmer.builder()
                .name(name)
                .isTemperatureNormal(true)
                .build());
    }

    public Farmer readFarmer(final Long id) {
        return farmerRepository.findById(id).orElse(null);
    }

    public Farmer readFarmerByName(final String name) {
        return farmerRepository.findByName(name);
    }

    public void updateFarmer(final Long id, final boolean isTemperatureNormal, final Double latitude, final Double longitude) {
        final Farmer farmer = readFarmer(id);
        if (! isTemperatureNormal) {
            farmer.update(false, null);
        } else {
            Long areaId = areaFarmerService.getAreaIdByGps(latitude, longitude);
            farmer.update(true, areaId);
        }
    }

}
