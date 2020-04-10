open DbConnexion;

module Messages = {
  let getAll = () =>
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.fromTable("forum_messages")
        |> Knex.toPromise
        |> then_(results => {
          Messages.Messageslist.fromJson(results)
             |> List.map(messages => {
                  Messages.Messages.make(
                    Messages.Messages.getIdMessage(messages),
                    Messages.Messages.getTexte(messages),
                    Messages.Messages.getDateEnvoi(messages),
                    Messages.Messages.getIdCours(messages),
                    Messages.Messages.getAuteur(messages),
                    Messages.Messages.getNbLikes(messages),
                  )
                })
             |> Messages.Messageslist.toJson
             |> resolve
           })
      );
  
  let getAllByCours : string => Js.Promise.t(Js.Json.t) =
  cours =>
      Js.Promise.(
        Dbconnexion.knex  
        |> Knex.fromTable("forum_messages")
        |> Knex.where({"forum_messages.idcours": cours})
        |> Knex.orderBy("dateenvoi")
        |> Knex.toPromise
        |> then_(results => {
              Messages.Messageslist.fromJson(results)
              |> List.map(messages => {
                  Messages.Messages.make(
                    Messages.Messages.getIdMessage(messages),
                    Messages.Messages.getTexte(messages),
                    Messages.Messages.getDateEnvoi(messages),
                    Messages.Messages.getIdCours(messages),
                    Messages.Messages.getAuteur(messages),
                    Messages.Messages.getNbLikes(messages),
                  )
                })
              |> Messages.Messageslist.toJson
              |> resolve
            })
      );
  
  let create = ( texte, idCours, auteur) => {
      let messages = Messages.Messages.make(202, texte, Js.Date.toISOString(Js.Date.make()), idCours, auteur,0);
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.rawBinding(
             "INSERT INTO forum_messages VALUES ( 202 , ? , now() + interval '2 hours' , ? , ?)",
             (
              Messages.Messages.getTexte(messages),
              Messages.Messages.getIdCours(messages),
              Messages.Messages.getAuteur(messages),
             ),
           )
        |> Knex.toPromise
        |> then_(_ => {resolve()})
      );
    };

  
};

