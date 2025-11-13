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
    sai_object_id_t         lag_oid_1;
    sai_object_id_t         lag_oid_2;
    sai_object_id_t         lag_member_oid_1;
    sai_object_id_t         lag_member_oid_2;
    sai_object_id_t         lag_member_oid_3;
    sai_object_id_t         lag_member_oid_4;
    sai_attribute_t         lag_1_attr[2];
    sai_attribute_t         lag_2_attr[2];
    sai_attribute_t         lag_member_1_attr[2];
    sai_attribute_t         lag_member_2_attr[2];
    sai_attribute_t         lag_member_3_attr[2];
    sai_attribute_t         lag_member_4_attr[2];
    sai_attribute_t         attr;

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

    // Create LAG#1
    status = lag_api->create_lag(&lag_oid_1, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG #1, status=%d\n", status);
        return -1;
    }

    // Create LAG_MEMBER#1 {LAG_ID:LAG#1, PORT_ID:PORT#1}
    lag_member_1_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_1_attr[0].value.oid = lag_oid_1;
    lag_member_1_attr[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_1_attr[1].value.oid = 1;
    status = lag_api->create_lag_member(&lag_member_oid_1, 2, lag_member_1_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG member #1, status=%d\n", status);
        return -1;
    }

    // Create LAG_MEMBER#2 {LAG_ID:LAG#1, PORT_ID:PORT#2}
    lag_member_2_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_2_attr[0].value.oid = lag_oid_1;
    lag_member_2_attr[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_2_attr[1].value.oid = 2;
    status = lag_api->create_lag_member(&lag_member_oid_2, 2, lag_member_2_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG member #2, status=%d\n", status);
        return -1;
    }

    // Create LAG#2
    status = lag_api->create_lag(&lag_oid_2, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG #2, status=%d\n", status);
        return -1;
    }

    // Create LAG_MEMBER#3 {LAG_ID:LAG#2, PORT_ID:PORT#3}
    lag_member_3_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_3_attr[0].value.oid = lag_oid_2;
    lag_member_3_attr[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_3_attr[1].value.oid = 3;
    status = lag_api->create_lag_member(&lag_member_oid_3, 2, lag_member_3_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG member #3, status=%d\n", status);
        return -1;
    }

    // Create LAG_MEMBER#4 {LAG_ID:LAG#2, PORT_ID:PORT#4}
    lag_member_4_attr[0].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    lag_member_4_attr[0].value.oid = lag_oid_2;
    lag_member_4_attr[1].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    lag_member_4_attr[1].value.oid = 4;
    status = lag_api->create_lag_member(&lag_member_oid_4, 2, lag_member_4_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create a LAG member #4, status=%d\n", status);
        return -1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1, PORT#2)]
    status = lag_api->get_lag_attribute(lag_oid_1, 2, lag_1_attr);
    printf("PORT#%ld PORT#%ld\n", lag_member_1_attr[1].value.oid, lag_member_2_attr[1].value.oid);

    // Get LAG#2 PORT_LIST [Expected: (PORT#3, PORT#4)]
    status = lag_api->get_lag_attribute(lag_oid_2, 2, lag_2_attr);
    printf("PORT#%ld PORT#%ld\n", lag_member_3_attr[1].value.oid, lag_member_4_attr[1].value.oid);

    // Get LAG_MEMBER#1 LAG_ID [Expected: LAG#1]
    attr.id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    status = lag_api->get_lag_attribute(lag_oid_1, 1, &attr);
    printf("LAG_ID: %ld\n", lag_member_1_attr[0].value.oid);

    // Get LAG_MEMBER#3 PORT_ID [Expected: PORT#3]
    attr.id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    status = lag_api->get_lag_attribute(lag_oid_2, 1, &attr);
    printf("PORT_ID: %ld\n", lag_member_3_attr[1].value.oid);

    // Remove LAG_MEMBER#2
    status = lag_api->remove_lag_member(lag_member_oid_2);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG member #2, status=%d\n", status);
        return -1;
    }

    // Get LAG#1 PORT_LIST [Expected: (PORT#1)]
    status = lag_api->get_lag_attribute(lag_oid_1, 2, lag_1_attr);
    printf("PORT#%ld\n", lag_member_1_attr[1].value.oid);

    // Remove LAG_MEMBER#3
    status = lag_api->remove_lag_member(lag_member_oid_3);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG member #3, status=%d\n", status);
        return -1;
    }

    // Get LAG#2 PORT_LIST [Expected: (PORT#4)]
    status = lag_api->get_lag_attribute(lag_oid_2, 2, lag_2_attr);
    printf("PORT#%ld\n", lag_member_4_attr[1].value.oid);

    // Remove LAG_MEMBER#1
    status = lag_api->remove_lag_member(lag_member_oid_1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG member #1, status=%d\n", status);
        return -1;
    }


    // Remove LAG_MEMBER#4
    status = lag_api->remove_lag_member(lag_member_oid_4);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG member #4, status=%d\n", status);
        return -1;
    }


    // Remove LAG#2
    status = lag_api->remove_lag(lag_oid_2);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG #2, status=%d\n", status);
        return -1;
    }

    // Remove LAG#1
    status = lag_api->remove_lag(lag_oid_1);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to remove a LAG #1, status=%d\n", status);
        return -1;
    }
    
    status = sai_api_uninitialize();
    if (status != SAI_STATUS_SUCCESS) {
        fprintf(stderr, "sai_api_uninitialize failed: %d", status);
        return -1;
    }

    return 0;
}