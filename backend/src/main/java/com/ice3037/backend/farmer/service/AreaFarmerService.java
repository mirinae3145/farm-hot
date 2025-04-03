package com.ice3037.backend.farmer.service;

import com.ice3037.backend.area.AreaService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class AreaFarmerService {

    private final AreaService areaService;

    public Long getAreaIdByGps(final Double latitude, final Double longitude) {
        return areaService.getAreaIdByGps(latitude, longitude);
    }

}
