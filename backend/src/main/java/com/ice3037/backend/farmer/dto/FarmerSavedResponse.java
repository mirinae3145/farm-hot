package com.ice3037.backend.farmer.dto;

import com.ice3037.backend.farmer.domain.Farmer;

public record FarmerSavedResponse(
    Long id,
    String name,
    boolean isTemperatureNormal
) {
    public static FarmerSavedResponse of(final Farmer farmer) {
        return new FarmerSavedResponse(farmer.getId(), farmer.getName(), farmer.isTemperatureNormal());
    }
}
