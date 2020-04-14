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
                    2,
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
              Js.log(results)
              Messages.Messageslist.fromJson(results)
              |> List.map(messages => {
                  Messages.Messages.make(
                    Messages.Messages.getIdMessage(messages),
                    Messages.Messages.getTexte(messages),
                    Messages.Messages.getDateEnvoi(messages),
                    Messages.Messages.getIdCours(messages),
                    Messages.Messages.getAuteur(messages),
                    1,
                  )
                })
              |> Messages.Messageslist.toJson
              |> resolve
            })
      );


      let deleteMessage = (idMessage) => {
        let messages = Messages.Messages.make(idMessage, "texte", Js.Date.toISOString(Js.Date.make()), "idCours", "auteur",0);
        ignore(LikesDAO.Likes.deleteAllLikesOfAMessage(idMessage));
        Js.Promise.(
          Dbconnexion.knex
          |> Knex.rawBinding(
               "DELETE FROM forum_messages where idmessage = ?",
               (
                Messages.Messages.getIdMessage(messages),
               ),
             )
          |> Knex.toPromise
          |> then_(_ => {resolve()})
        );
      };
  
  


      /*
  let decodeInt = json =>
      switch (Js.Json.classify(json)) {
        | Js.Json.JSONNumber(float) => Ok(int_of_float(float))
        | _ => Error("Not an int")
    };

  let handleValue =
  Js.Promise.then_(
    (result) =>
      Js.Promise.resolve(
        switch result {
        | Ok(value) => value
        | Error(reason) => -1
        }
      )
  );


      
  let getNextIdInBase = ()  =>
  Js.Promise.(
    Dbconnexion.knex
    |> Knex.raw("select max(idMessage) as value from forum_messages")
    |> then_(json => {
                 let myInt = decodeInt(json); 
                 /* do something with myInt*/
                 resolve();/* ici ça retourne Promise(unit) mais tu peux retourner n'importe quelle Promise('a) selon le type attendu en retour */
                 })
  );

*/
  
  let create = ( texte, idCours, auteur) => {
      let messages = Messages.Messages.make(0, texte, Js.Date.toISOString(Js.Date.make()), idCours, auteur,0);
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.rawBinding(
             "INSERT INTO forum_messages VALUES ( (select max(idMessage)+1 from forum_messages) , ? , ? , ? , ?)",
             (
              Messages.Messages.getTexte(messages),
              Messages.Messages.getDateEnvoi(messages),
              Messages.Messages.getIdCours(messages),
              Messages.Messages.getAuteur(messages),
             ),
           )
        |> Knex.toPromise
        |> then_(_ => {resolve()})
      );
    };


};

