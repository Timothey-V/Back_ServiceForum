open DbConnexion;

module Channels = {
    let create = (coursname) => {
      let channels = Channels.Channels.make(0,coursname);
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.rawBinding(
             "INSERT INTO forum_channels (coursname) VALUES (?)",
             (
              Channels.Channels.getCoursName(channels),
             ),
           )
        |> Knex.toPromise
        |> then_(_ => {resolve()})
        |> catch(_ => {
          resolve();
        })      
        );
    };

    let getAll = () =>
    Js.Promise.(
      Dbconnexion.knex
      |> Knex.fromTable("forum_channels")
      |> Knex.toPromise
      |> then_(results => {
        Channels.Channelslist.fromJson(results)
           |> List.map(channels => {
                Channels.Channels.make(
                  Channels.Channels.getId(channels),
                  Channels.Channels.getCoursName(channels),
                )
              })
           |> Channels.Channelslist.toJson
           |> resolve
         })
    );

};

