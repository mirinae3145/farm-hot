package com.ice3037.backend.farmer.dto;


public record FarmerUpdateRequest(
       boolean isTemperatureNormal,
       Double latitude,
       Double longitude
) {
}
