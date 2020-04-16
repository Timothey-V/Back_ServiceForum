let bdd_host : string = "bz24tpiucfm0qm0sm1qz-postgresql.services.clever-cloud.com";
let bdd_user : string = "ur11qcusccvyqokg98iw";
let bdd_password : string = "WeRXzaeor8fdR1cMW3WQ";
let bdd_db_name : string = "bz24tpiucfm0qm0sm1qz";

module Dbconnexion = {
    let connection =
      KnexConfig.Connection.make(
      ~host=bdd_host,
      ~user=bdd_user,
      ~password=bdd_password,
      ~database=bdd_db_name,
      ()
      );

    let config =
      KnexConfig.make(
        ~client="pg",
        ~version="11",
        ~connection,
        ~acquireConnectionTimeout=20000,
        (),
      );

    let knex = Knex.make(config);
}
