//
//  hb_mongoc_bulk_operation.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/7/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_bulk_operation.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_BULK_OPERATION_DESTROY )
{
    PHB_MONGOC phBulk = hbmongoc_param( 1, _hbmongoc_bult_operation_t_ );

    if ( phBulk ) {
        mongoc_bulk_operation_destroy( ( mongoc_bulk_operation_t * ) phBulk->p );
        phBulk->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_EXECUTE )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bult_operation_t_ );

    if ( bulk && HB_ISBYREF( 2 ) ) {
        bson_t * reply = bson_new();
        bson_error_t error;

        uint32_t result = mongoc_bulk_operation_execute( bulk, reply, &error );

        hbmongoc_return_byref_bson( 2, reply );

        if ( HB_ISBYREF( 3 ) ) {
            bson_hbstor_ref_error( 3, &error );
        }

        hb_retni( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_INSERT )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bult_operation_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );

    if ( bulk && document ) {
        mongoc_bulk_operation_insert( bulk, document );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_BULK_OPERATION_INSERT_WITH_OPTS )
{
    mongoc_bulk_operation_t * bulk = mongoc_hbparam( 1, _hbmongoc_bult_operation_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );
    bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

    if ( bulk && document && opts ) {
        bson_error_t error;

        HB_BOOL result = mongoc_bulk_operation_insert_with_opts( bulk, document, opts, &error );

        if ( HB_ISBYREF( 4 ) ) {
            if ( result ) {
                hb_stor( 4 );
            } else {
                bson_hbstor_ref_error( 4, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( document );
    }

    if ( opts && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( opts );
    }
}
