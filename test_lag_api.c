#include <stdio.h>
#include "sai.h"
#include "stub_sai.h"

const char* test_profile_get_value(
    _In_ sai_switch_profile_id_t profile_id,
    _In_ const char* variable)
{
    return 0;
}

int test_profile_get_next_value(
    _In_ sai_switch_profile_id_t profile_id,
    _Out_ const char** variable,
    _Out_ const char** value)
{
    return -1;
}

const service_method_table_t test_services = {
    test_profile_get_value,
    test_profile_get_next_value
};

int main() 
{
    sai_status_t             status;
    const sai_lag_api_t     *lag_api;
    sai_object_id_t         lag_oid;
    sai_object_id_t         lag_member_oid;

    status = sai_api_initialize(0, &test_services);
    if (status != SAI_STATUS_SUCCESS) {
        fprintf(stderr, "sai_api_initialize failed: %d", status);
        return -1;
    }

    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return -1;
    }

    status = lag_api->create_lag(&lag_oid, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG, status=%d\n", status);
        return -1;
    }

    status = lag_api->create_lag_member(&lag_member_oid, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG member, status=%d\n", status);
        return -1;
    }

    status = lag_api->remove_lag_member(lag_member_oid);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG member, status=%d\n", status);
        return -1;
    }

    status = lag_api->remove_lag(lag_oid);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG, status=%d\n", status);
        return -1;
    }

    status = sai_api_uninitialize();
    if (status != SAI_STATUS_SUCCESS) {
        fprintf(stderr, "sai_api_uninitialize failed: %d", status);
        return -1;
    }

    return 0;
}