#include <stdio.h>
#include <string.h>
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
    sai_status_t         status;
    const sai_lag_api_t *lag_api;
    sai_object_id_t      lag_oid;
    sai_object_id_t      lag_member_oid;
    sai_object_id_t      port_oid;

    // Initialize SAI
    status = sai_api_initialize(0, &test_services);
    if (status != SAI_STATUS_SUCCESS) {
        printf("sai_api_initialize failed: %d\n", status);
        return -1;
    }

    // Query LAG API
    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query LAG API, status=%d\n", status);
        return -1;
    }

    // Create a dummy port (required for LAG_MEMBER)
    status = stub_create_object(SAI_OBJECT_TYPE_PORT, 1, &port_oid);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create PORT OID, status=%d\n", status);
        return -1;
    }

    // Create LAG
    status = lag_api->create_lag(&lag_oid, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG, status=%d\n", status);
        return -1;
    }

    // Create LAG_MEMBER with attributes
    sai_attribute_t member_attrs[2];
    member_attrs[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    member_attrs[0].value.oid = lag_oid;

    member_attrs[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    member_attrs[1].value.oid = port_oid;

    status = lag_api->create_lag_member(&lag_member_oid, 2, member_attrs);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG member, status=%d\n", status);
        return -1;
    }

    // Attempt to remove LAG (should fail because member still exists)
    status = lag_api->remove_lag(lag_oid);
    if (status == SAI_STATUS_SUCCESS) {
        printf("ERROR: removing LAG should fail while it still has members\n");
        return -1;
    }

    // Remove LAG_MEMBER
    status = lag_api->remove_lag_member(lag_member_oid);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG member, status=%d\n", status);
        return -1;
    }

    // Remove LAG (should succeed now)
    status = lag_api->remove_lag(lag_oid);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove LAG, status=%d\n", status);
        return -1;
    }

    // Uninitialize
    status = sai_api_uninitialize();
    if (status != SAI_STATUS_SUCCESS) {
        printf("sai_api_uninitialize failed: %d\n", status);
        return -1;
    }

    return 0;
}